#include "ConfigManager.h"
#include "ConfigResult.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLoggingCategory> // New include
#include <QRecursiveMutex>
#include <QStandardPaths>
#include <QStringConverter>
#include <QTextStream>
#include <QtGlobal>
#include <utility>

Q_LOGGING_CATEGORY(lcConfigManager,
                   "mpvdeck.configmanager") // New logging category

ConfigManager::ConfigManager(QString configFilePath)
    : m_configFilePath(std::move(configFilePath))
{
}

auto ConfigManager::findConfigFile() const -> QString
{
  if(!m_configFilePath.isEmpty())
  {
    return m_configFilePath;
  }

  // Get the standard config location using Qt's helper functions
  QString configPath =
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  if(configPath.isEmpty())
  {
    // Fallback to the home directory if XDG_CONFIG_HOME is not set
    configPath = QDir::homePath() + "/.config";
  }

  QDir mpvConfigDir(configPath + "/mpv");

  // Always return the expected path, even if the file doesn't exist yet.
  return mpvConfigDir.filePath("mpv.conf");
}

auto ConfigManager::configFileExists() const -> bool
{
  QString pathToCheck = m_configFilePath;
  if(pathToCheck.isEmpty())
  {
    pathToCheck = findConfigFile();
  }
  return QFile::exists(pathToCheck);
}

static QString unescapeFromQuotes(const QString& v)
{
  QString out = v;
  out.replace(QStringLiteral("\\\""), QStringLiteral("\""));
  out.replace(QStringLiteral("\\\\"), QStringLiteral("\\"));
  return out;
}

auto ConfigManager::parseLine(const QString& line) -> ConfigLine
{
  ConfigLine configLine;

  if(line.length() > MAX_CONFIG_LINE_LENGTH)
  {
    configLine.type         = ConfigManager::ConfigLineType::Invalid;
    configLine.rawText      = line.left(MAX_CONFIG_LINE_LENGTH) + "...";
    configLine.errorMessage = "Line exceeds maximum allowed length of " +
                              QString::number(MAX_CONFIG_LINE_LENGTH) +
                              " characters.";
    return configLine;
  }

  QString trimmedLine = line.trimmed();

  if(trimmedLine.isEmpty())
  {
    configLine.type    = ConfigManager::ConfigLineType::EmptyLine;
    configLine.rawText = line; // Preserve original indentation for empty lines
    return configLine;
  }

  if(trimmedLine.startsWith('#'))
  {
    configLine.type    = ConfigManager::ConfigLineType::Comment;
    configLine.rawText = line; // Preserve original indentation for comments
    return configLine;
  }

  // Attempt to parse as KeyValuePair
  configLine.type = ConfigManager::ConfigLineType::KeyValuePair;
  configLine.rawText =
      line; // Store original line for potential comment preservation

  int  commentIndex = -1;
  bool inQuote      = false;
  for(int i = 0; i < static_cast<int>(line.length()); ++i)
  {
    if(line.at(i) == '"' && (i == 0 || line.at(i - 1) != '\\'))
    {
      inQuote = !inQuote;
    }
    else if(!inQuote && line.at(i) == '#')
    {
      commentIndex = i;
      break;
    }
  }

  QString effectiveLine;
  if(commentIndex != -1)
  {
    configLine.trailingComment = line.mid(commentIndex);
    effectiveLine              = line.left(commentIndex).trimmed();
  }
  else
  {
    effectiveLine = line.trimmed();
  }

  int separatorIndex = effectiveLine.indexOf('=');
  if(separatorIndex != -1)
  {
    configLine.key = effectiveLine.left(separatorIndex).trimmed();
    if(configLine.key.isEmpty())
    {
      configLine.type         = ConfigManager::ConfigLineType::Invalid;
      configLine.errorMessage = "Key cannot be empty in a key-value pair.";
      return configLine;
    }
    QString value = effectiveLine.mid(separatorIndex + 1).trimmed();

    // Remove quotes from value if present and unescape
    if(value.startsWith('"') && value.endsWith('"'))
    {
      value = value.mid(1, qMax(0, static_cast<int>(value.length()) - 2));
      value = unescapeFromQuotes(value);
    }
    configLine.value = value;
  }
  else
  {
    // The whole line is a flag, value is empty
    configLine.key = effectiveLine;
    if(configLine.key.isEmpty())
    {
      configLine.type = ConfigManager::ConfigLineType::Invalid;
      configLine.errorMessage =
          QString("Invalid configuration line: '%1'. Expected format: "
                  "'key=value' or 'key' for flags")
              .arg(line.trimmed());
      return configLine;
    }
    configLine.value = "";
  }

  return configLine;
}

static QString escapeForQuotes(const QString& v)
{
  QString out = v;
  out.replace('\\', "\\\\");
  out.replace('"', "\\\"");
  return out;
}

auto ConfigManager::serializeLine(const ConfigLine& line) -> QString
{
  QString serialized;
  switch(line.type)
  {
  case ConfigManager::ConfigLineType::EmptyLine:
  case ConfigManager::ConfigLineType::Comment:
    return line.rawText;
  case ConfigManager::ConfigLineType::KeyValuePair:
    serialized = line.key;
    if(!line.value.isEmpty())
    {
      // Check if value contains spaces or special characters that require
      // quoting
      if(line.value.contains(' ') || line.value.contains('#') ||
         line.value.contains('=') || line.value.contains('"') ||
         line.value.contains('\\'))
      {
        serialized += "=\"" + escapeForQuotes(line.value) + "\"";
      }
      else
      {
        serialized += "=" + line.value;
      }
    }
    else
    { // Value is empty, but it's a KeyValuePair, so append '=' for flags
      serialized += "=";
    }
    return serialized + line.trailingComment;
  case ConfigManager::ConfigLineType::Invalid:
    return "# ERROR: " + line.errorMessage + " -- Original: " + line.rawText;
  }
  return {}; // Should not happen
}

auto ConfigManager::readConfigFile() -> QList<ConfigLine>
{
  QMutexLocker locker(&m_mutex); // Lock for thread-safety
  m_configLines.clear();         // Clear previous state
  QString configFilePath = findConfigFile();
  if(configFilePath.isEmpty())
  {
    qCWarning(lcConfigManager) << "Config file path is not set.";
    return m_configLines;
  }

  QFile configFile(configFilePath);
  if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qCWarning(lcConfigManager)
        << "Could not open config file:" << configFilePath;
    return m_configLines;
  }

  QTextStream textStream(&configFile);
  textStream.setEncoding(QStringConverter::Utf8);
  while(!textStream.atEnd())
  {
    QString line = textStream.readLine(); // Read the entire line
    if(line.length() > MAX_CONFIG_LINE_LENGTH)
    {
      ConfigLine invalidLine;
      invalidLine.type = ConfigManager::ConfigLineType::Invalid;
      invalidLine.rawText =
          line.left(MAX_CONFIG_LINE_LENGTH) + "..."; // Truncate for display
      invalidLine.errorMessage = "Line exceeds maximum allowed length of " +
                                 QString::number(MAX_CONFIG_LINE_LENGTH) +
                                 " characters.";
      m_configLines.append(invalidLine);
    }
    else
    {
      m_configLines.append(ConfigManager::parseLine(line));
    }
  }

  configFile.close();
  return m_configLines;
}

auto ConfigManager::validateConfigLines() const -> QStringList
{
  QStringList errors;
  for(const ConfigLine& line : m_configLines)
  {
    if(line.type == ConfigManager::ConfigLineType::Invalid)
    {
      errors.append(QString("Invalid line: '%1' - %2")
                        .arg(line.rawText, line.errorMessage));
    }
  }
  return errors;
}

void ConfigManager::updateConfigLines(QMap<QString, QString>& newSettings)
{
  for(ConfigLine& line : m_configLines)
  {
    if(line.type == ConfigManager::ConfigLineType::KeyValuePair &&
       newSettings.contains(line.key))
    {
      line.value = newSettings.value(line.key);
      newSettings.remove(line.key);
    }
  }
  for(auto it = newSettings.constBegin(); it != newSettings.constEnd(); ++it)
  {
    ConfigLine newLine;
    newLine.type  = ConfigManager::ConfigLineType::KeyValuePair;
    newLine.key   = it.key();
    newLine.value = it.value();
    m_configLines.append(newLine);
  }
}

auto ConfigManager::writeConfigLines(const QString& tempConfigFilePath) const
    -> MPVDeck::ConfigResult
{
  QFile tempConfigFile(tempConfigFilePath);
  if(!tempConfigFile.open(QIODevice::WriteOnly | QIODevice::Text |
                          QIODevice::Truncate))
  {
    QString errorMessage =
        "Could not open temporary config file for writing: " +
        tempConfigFilePath + ", Error: " + tempConfigFile.errorString();
    qCWarning(lcConfigManager) << errorMessage;
    return MPVDeck::ConfigResult::failureResult(errorMessage);
  }

  QTextStream out(&tempConfigFile);
  out.setEncoding(QStringConverter::Utf8);
  for(const ConfigLine& line : m_configLines)
  {
    out << ConfigManager::serializeLine(line) << "\n";
  }
  tempConfigFile.close();
  return MPVDeck::ConfigResult::successResult();
}

auto ConfigManager::saveConfigFile(const QMap<QString, QString>& newSettings,
                                   bool reuseExistingLines)
    -> MPVDeck::ConfigResult
{
  QMutexLocker locker(&m_mutex); // Lock for thread-safety
  QString      configFilePath = findConfigFile();
  if(configFilePath.isEmpty())
  {
    qCWarning(lcConfigManager) << "Config file path is not set. Cannot save.";
    return MPVDeck::ConfigResult::failureResult(
        "Config file path is not set. Cannot save.");
  }

  QFileInfo fileInfo(configFilePath);
  QDir      configDir = fileInfo.dir();
  if(!configDir.exists())
  {
    qCDebug(lcConfigManager)
        << "Creating config directory:" << configDir.path();
    if(!configDir.mkpath("."))
    {
      QString errorMessage =
          "Failed to create config directory: " + configDir.path();
      qCWarning(lcConfigManager) << errorMessage;
      return MPVDeck::ConfigResult::failureResult(errorMessage);
    }
  }

  if(reuseExistingLines)
  {
    readConfigFile();
  }
  else
  {
    m_configLines.clear();
  }

  QStringList errors = validateConfigLines();
  if(!errors.isEmpty())
  {
    QString errorMessage =
        "Cannot save configuration due to invalid lines:" + errors.join("\n");
    qCWarning(lcConfigManager) << errorMessage;
    return MPVDeck::ConfigResult::failureResult(errorMessage);
  }

  QMap<QString, QString> settingsToApply = newSettings;
  updateConfigLines(settingsToApply);

  QString               tempConfigFilePath = configFilePath + ".tmp";
  MPVDeck::ConfigResult writeResult = writeConfigLines(tempConfigFilePath);
  if(!writeResult.success)
  {
    return writeResult;
  }

  // Atomically replace the old config file with the new one
  if(QFile::exists(configFilePath) && !QFile::remove(configFilePath))
  {
    QString errorMessage =
        "Could not remove existing config file: " + configFilePath +
        ", Error: " + QFile(configFilePath).errorString();
    qCWarning(lcConfigManager) << errorMessage;
    return MPVDeck::ConfigResult::failureResult(errorMessage);
  }

  if(!QFile::rename(tempConfigFilePath, configFilePath))
  {
    QString errorMessage =
        "Could not rename temporary config file to final config file: " +
        tempConfigFilePath + " -> " + configFilePath +
        ", Error: " + QFile(tempConfigFilePath).errorString();
    qCWarning(lcConfigManager) << errorMessage;
    return MPVDeck::ConfigResult::failureResult(errorMessage);
  }

  qCDebug(lcConfigManager) << "Config file saved successfully to:"
                           << configFilePath;
  return MPVDeck::ConfigResult::successResult();
}

auto ConfigManager::getSettingsMap() const -> QMap<QString, QString>
{
  QMap<QString, QString> settingsMap;
  for(const ConfigLine& line : m_configLines)
  {
    if(line.type == ConfigManager::ConfigLineType::KeyValuePair)
    {
      settingsMap[line.key] = line.value;
    }
  }
  return settingsMap;
}

auto ConfigManager::getRawConfig() const -> QString
{
  QStringList rawLines;
  for(const ConfigLine& line : m_configLines)
  {
    rawLines.append(ConfigManager::serializeLine(line));
  }
  return rawLines.join("\n");
}

auto ConfigManager::getParsedConfigLines() const -> const QList<ConfigLine>&
{
  return m_configLines;
}

auto ConfigManager::parseRawConfig(const QString& configText)
    -> MPVDeck::ConfigResult
{
  QMutexLocker locker(&m_mutex);
  m_configLines.clear();
  QStringList lines = configText.split('\n');
  for(const QString& line : lines)
  {
    m_configLines.append(ConfigManager::parseLine(line));
  }

  QStringList errors;
  for(const ConfigLine& line : m_configLines)
  {
    if(line.type == ConfigManager::ConfigLineType::Invalid)
    {
      errors.append(QString("Invalid line: '%1' - %2")
                        .arg(line.rawText, line.errorMessage));
    }
  }

  if(!errors.isEmpty())
  {
    return MPVDeck::ConfigResult::failureResult("Invalid lines detected:\n" +
                                                errors.join("\n"));
  }
  return MPVDeck::ConfigResult::successResult();
}