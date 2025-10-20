#include "ConfigManager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>
#include <QStringConverter>

// Removed constructor

ConfigManager::ConfigManager(const QString& configFilePath) : m_configFilePath(configFilePath) {}

auto ConfigManager::findConfigFile() -> QString {
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
    pathToCheck = const_cast<ConfigManager*>(this)->findConfigFile();
  }
  return QFile::exists(pathToCheck);
}

auto ConfigManager::parseLine(const QString& line) -> ConfigLine {
    ConfigLine configLine;
    QString trimmedLine = line.trimmed();

    if (trimmedLine.isEmpty()) {
        configLine.type = EmptyLine;
        configLine.rawText = line; // Preserve original indentation for empty lines
        return configLine;
    }

    if (trimmedLine.startsWith('#')) {
        configLine.type = Comment;
        configLine.rawText = line; // Preserve original indentation for comments
        return configLine;
    }

    // Attempt to parse as KeyValuePair
    configLine.type = KeyValuePair;
    configLine.rawText = line; // Store original line for potential comment preservation

    int commentIndex = -1;
    bool inQuote = false;
    for (int i = 0; i < line.length(); ++i) {
        if (line.at(i) == '"') {
            inQuote = !inQuote;
        } else if (!inQuote && line.at(i) == '#') {
            if (i == 0 || line.at(i - 1).isSpace()) { // Only consider # as comment if preceded by space or at start
                commentIndex = i;
                break;
            }
        }
    }

    QString effectiveLine = (commentIndex != -1) ? line.left(commentIndex).trimmed() : line.trimmed();

    int separatorIndex = effectiveLine.indexOf('=');
    if (separatorIndex != -1) {
        configLine.key = effectiveLine.left(separatorIndex).trimmed();
        QString value = effectiveLine.mid(separatorIndex + 1).trimmed();

        // Remove quotes from value if present
        if (value.startsWith('"') && value.endsWith('"')) {
            value = value.mid(1, value.length() - 2);
        }
        configLine.value = value;
    } else {
        // The whole line is a flag, value is empty
        configLine.key = effectiveLine;
        configLine.value = "";
    }

    return configLine;
}

auto ConfigManager::serializeLine(const ConfigLine& line) const -> QString {
    switch (line.type) {
        case EmptyLine:
        case Comment:
            return line.rawText;
        case KeyValuePair:
            if (line.value.isEmpty()) {
                return line.key; // For flags
            } else {
                // Check if value contains spaces or special characters that require quoting
                if (line.value.contains(' ') || line.value.contains('#') || line.value.contains('=')) {
                    return line.key + "=\"" + line.value + "\"";
                }
                return line.key + "=" + line.value;
            }
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
        QString line = textStream.readLine(); // Read original line to preserve indentation/comments
        m_configLines.append(parseLine(line));
    }

    configFile.close();
    return m_configLines;
}

auto ConfigManager::saveConfigFile(const QMap<QString, QString> &newSettings) -> bool {
    QString configFilePath = findConfigFile();
    if (configFilePath.isEmpty()) {
        qWarning() << "Config file path is not set. Cannot save.";
        return false;
    }

    QFileInfo fileInfo(configFilePath);
    QDir configDir = fileInfo.dir();
    if (!configDir.exists()) {
        qDebug() << "Creating config directory:" << configDir.path();
        if (!configDir.mkpath(".")) {
            qWarning() << "Failed to create config directory:" << configDir.path();
            return false;
        }
    }

    // Read the current file to get the latest structure including comments
    readConfigFile(); // This populates m_configLines

    QMap<QString, QString> settingsToApply = newSettings; // Make a mutable copy

    // Update existing settings in m_configLines
    for (ConfigLine& line : m_configLines) {
        if (line.type == KeyValuePair && settingsToApply.contains(line.key)) {
            line.value = settingsToApply.value(line.key);
            settingsToApply.remove(line.key);
        }
    }

    // Add any new settings that were not in the original file
    for (auto it = settingsToApply.constBegin(); it != settingsToApply.constEnd(); ++it) {
        ConfigLine newLine;
        newLine.type = KeyValuePair;
        newLine.key = it.key();
        newLine.value = it.value();
        m_configLines.append(newLine);
    }

    QFile configFile(configFilePath);
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open config file for writing:" << configFilePath << ", Error:" << configFile.errorString();
        return false;
    }

    QTextStream out(&configFile);
    out.setEncoding(QStringConverter::Utf8);
    for (const ConfigLine& line : m_configLines) {
        out << serializeLine(line) << "\n";
    }

    configFile.close();
    qDebug() << "Config file saved successfully to:" << configFilePath;
    return true;
}

auto ConfigManager::getSettingsMap() const -> QMap<QString, QString> {
    QMap<QString, QString> settingsMap;
    for (const ConfigLine& line : m_configLines) {
        if (line.type == KeyValuePair) {
            settingsMap[line.key] = line.value;
        }
    }
    return settingsMap;
}

QString ConfigManager::getRawConfig() const {
    QStringList rawLines;
    for (const ConfigLine& line : m_configLines) {
        rawLines.append(serializeLine(line));
    }
    return rawLines.join("\n");
}

void ConfigManager::parseRawConfig(const QString &configText) {
    m_configLines.clear();
    QStringList lines = configText.split('\n');
    for (const QString& line : lines) {
        m_configLines.append(parseLine(line));
    }
}
