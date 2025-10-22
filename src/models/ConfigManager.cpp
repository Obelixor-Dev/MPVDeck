#include "ConfigManager.h"
#include "ConfigResult.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QStringConverter>
#include <QTextStream>
#include <utility>
#include <QtGlobal>

// Removed constructor

ConfigManager::ConfigManager(QString configFilePath)
    : m_configFilePath(std::move(configFilePath)) {}

auto ConfigManager::findConfigFile() const -> QString {
  if (!m_configFilePath.isEmpty()) {
    return m_configFilePath;
  }

  // Get the standard config location using Qt's helper functions
  QString configPath =
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  if (configPath.isEmpty()) {
    // Fallback to the home directory if XDG_CONFIG_HOME is not set
    configPath = QDir::homePath() + "/.config";
  }

  QDir mpvConfigDir(configPath + "/mpv");

  // Always return the expected path, even if the file doesn't exist yet.
  return mpvConfigDir.filePath("mpv.conf");
}

auto ConfigManager::configFileExists() const -> bool {
  QString pathToCheck = m_configFilePath;
  if (pathToCheck.isEmpty()) {
    pathToCheck = findConfigFile();
  }
  return QFile::exists(pathToCheck);
}

auto ConfigManager::parseLine(const QString &line) -> ConfigLine {
  ConfigLine configLine;
  QString trimmedLine = line.trimmed();

  if (trimmedLine.isEmpty()) {
    configLine.type = ConfigManager::ConfigLineType::EmptyLine;
    configLine.rawText = line; // Preserve original indentation for empty lines
    return configLine;
  }

  if (trimmedLine.startsWith('#')) {
    configLine.type = ConfigManager::ConfigLineType::Comment;
    configLine.rawText = line; // Preserve original indentation for comments
    return configLine;
  }

  // Attempt to parse as KeyValuePair
  configLine.type = ConfigManager::ConfigLineType::KeyValuePair;
  configLine.rawText =
      line; // Store original line for potential comment preservation

  int commentIndex = -1;
  bool inQuote = false;
  for (int i = 0; i < static_cast<int>(line.length()); ++i) {
    if (line.at(i) == '"') {
      inQuote = !inQuote;
    } else if (!inQuote && line.at(i) == '#') {
      if (i == 0 || line.at(i - 1).isSpace()) { // Only consider # as comment if
                                                // preceded by space or at start
        commentIndex = i;
        break;
      }
    }
  }

  QString effectiveLine =
      (commentIndex != -1) ? line.left(commentIndex).trimmed() : line.trimmed();

  int separatorIndex = effectiveLine.indexOf('=');
  if (separatorIndex != -1) {
    configLine.key = effectiveLine.left(separatorIndex).trimmed();
    QString value = effectiveLine.mid(separatorIndex + 1).trimmed();

    // Remove quotes from value if present
    if (value.startsWith('"') && value.endsWith('"')) {
      value = value.mid(1, qMax(0, static_cast<int>(value.length()) - 2)); // NOLINT(bugprone-narrowing-conversions)
    }
    configLine.value = value;
  } else {
    // The whole line is a flag, value is empty
    configLine.key = effectiveLine; // NOLINT(bugprone-narrowing-conversions)
    configLine.value = "";
  }

  return configLine;
}

auto ConfigManager::serializeLine(const ConfigLine &line) -> QString {
  switch (line.type) {
  case ConfigManager::ConfigLineType::EmptyLine:
  case ConfigManager::ConfigLineType::Comment:
    return line.rawText;
  case ConfigManager::ConfigLineType::KeyValuePair:
    if (line.value.isEmpty()) {
      return line.key; // For flags
    }
    // Check if value contains spaces or special characters that require
    // quoting
    if (line.value.contains(' ') || line.value.contains('#') ||
        line.value.contains('=')) {
      return line.key + "=\"" + line.value + "\"";
    }
    return line.key + "=" + line.value;
  }
  return {}; // Should not happen
}

auto ConfigManager::readConfigFile() -> QList<ConfigLine> {
  m_configLines.clear(); // Clear previous state
  QString configFilePath = findConfigFile();
  if (configFilePath.isEmpty()) {
    qWarning() << "Config file path is not set.";
    return m_configLines;
  }

  QFile configFile(configFilePath);
  if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Could not open config file:" << configFilePath;
    return m_configLines;
  }

  QTextStream textStream(&configFile);
  textStream.setEncoding(QStringConverter::Utf8);
  while (!textStream.atEnd()) {
    QString line =
        textStream
            .readLine(); // Read original line to preserve indentation/comments
    m_configLines.append(ConfigManager::parseLine(line));
  }

  configFile.close();
  return m_configLines;
}

auto ConfigManager::saveConfigFile(const QMap<QString, QString> &newSettings,
                                   bool reuseExistingLines)
    -> MPVDeck::ConfigResult {
  QString configFilePath = findConfigFile();
  if (configFilePath.isEmpty()) {
    qWarning() << "Config file path is not set. Cannot save.";
    return MPVDeck::ConfigResult::failureResult(
        "Config file path is not set. Cannot save.");
  }

  QFileInfo fileInfo(configFilePath);
  QDir configDir = fileInfo.dir();
  if (!configDir.exists()) {
    qDebug() << "Creating config directory:" << configDir.path();
    if (!configDir.mkpath(".")) {
      QString errorMessage =
          "Failed to create config directory: " + configDir.path();
      qWarning() << errorMessage;
      return MPVDeck::ConfigResult::failureResult(errorMessage);
    }
  }

  // Read the current file to get the latest structure including comments
  if (!reuseExistingLines) {
    readConfigFile(); // This populates m_configLines
  }

  QMap<QString, QString> settingsToApply = newSettings; // Make a mutable copy

  // Update existing settings in m_configLines
  for (ConfigLine &line : m_configLines) {
    if (line.type == ConfigManager::ConfigLineType::KeyValuePair && settingsToApply.contains(line.key)) {
      line.value = settingsToApply.value(line.key);
      settingsToApply.remove(line.key);
    }
  }

  // Add any new settings that were not in the original file
  for (auto it = settingsToApply.constBegin(); it != settingsToApply.constEnd();
       ++it) {
    ConfigLine newLine;
    newLine.type = ConfigManager::ConfigLineType::KeyValuePair;
    newLine.key = it.key();
    newLine.value = it.value();
    m_configLines.append(newLine);
  }

  QFile configFile(configFilePath);
  if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text |
                       QIODevice::Truncate)) {
    QString errorMessage =
        "Could not open config file for writing: " + configFilePath +
        ", Error: " + configFile.errorString();
    qWarning() << errorMessage;
    return MPVDeck::ConfigResult::failureResult(errorMessage);
  }

  QTextStream out(&configFile);
  out.setEncoding(QStringConverter::Utf8);
  for (const ConfigLine &line : m_configLines) {
    out << ConfigManager::serializeLine(line) << "\n";
  }

  configFile.close();
  qDebug() << "Config file saved successfully to:" << configFilePath;
  return MPVDeck::ConfigResult::successResult();
}

auto ConfigManager::getSettingsMap() const -> QMap<QString, QString> {
  QMap<QString, QString> settingsMap;
  for (const ConfigLine &line : m_configLines) {
    if (line.type == ConfigManager::ConfigLineType::KeyValuePair) {
      settingsMap[line.key] = line.value;
    }
  }
  return settingsMap;
}

auto ConfigManager::getRawConfig() const -> QString {
  QStringList rawLines;
  for (const ConfigLine &line : m_configLines) {
    rawLines.append(ConfigManager::serializeLine(line));
  }
  return rawLines.join("\n");
}

void ConfigManager::parseRawConfig(const QString &configText) {
  m_configLines.clear();
  QStringList lines = configText.split('\n');
  for (const QString &line : lines) {
    m_configLines.append(ConfigManager::parseLine(line));
  }
}
