#include "ConfigManager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

// Removed constructor

auto ConfigManager::findConfigFile() -> QString {
  // Get the standard config location using Qt's helper functions
  QString configPath =
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  if (configPath.isEmpty()) {
    // Fallback to the home directory if XDG_CONFIG_HOME is not set
    configPath = QDir::homePath() + "/.config";
  }

  QDir mpvConfigDir(configPath + "/mpv");

  if (mpvConfigDir.exists("mpv.conf")) {
    return mpvConfigDir.filePath("mpv.conf");
  }

  // Return an empty string if not found
  return {};
}

auto ConfigManager::readConfigFile() -> QMap<QString, QString> {
  QMap<QString, QString> settings;
  QString configFilePath = findConfigFile(); // Call static method
  if (configFilePath.isEmpty()) {
    qWarning() << "Config file path is not set.";
    return settings;
  }

  QFile configFile(configFilePath);
  if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Could not open config file:" << configFilePath;
    return settings;
  }

  QTextStream textStream(&configFile);
  while (!textStream.atEnd()) {
    QString line = textStream.readLine().trimmed();

    // Ignore comments and empty lines
    if (line.isEmpty() || line.startsWith('#')) {
      continue;
    }

    // Strip inline comments
    int commentIndex = static_cast<int>(line.indexOf('#'));
    if (commentIndex != -1) {
      line = line.left(commentIndex).trimmed();
    }

    if (line.isEmpty()) {
      continue;
    }

    int separatorIndex = static_cast<int>(line.indexOf('='));
    if (separatorIndex != -1) {
      QString key = line.left(separatorIndex).trimmed();
      QString value = line.mid(separatorIndex + 1).trimmed();

      // Remove quotes from value
      if (value.startsWith('"') && value.endsWith('"')) {
        value = value.mid(1, value.length() - 2);
      }

      if (!key.isEmpty()) {
        settings[key] = value;
      }
    } else {
      // The whole line is a flag
      settings[line.trimmed()] = "";
    }
  }

  return settings;
}

auto ConfigManager::saveConfigFile(const QMap<QString, QString> &settings) -> bool {
  QString configFilePath = findConfigFile(); // Call static method
  if (configFilePath.isEmpty()) {
    qWarning() << "Config file path is not set. Cannot save.";
    return false;
  }

  QFile configFile(configFilePath);
  if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text |
                       QIODevice::Truncate)) {
    qWarning() << "Could not open config file for writing:" << configFilePath;
    return false;
  }

  // WARNING: This implementation overwrites the entire file and loses all
  // comments.
  QTextStream out(&configFile);
  for (auto it = settings.constBegin(); it != settings.constEnd(); ++it) {
    out << it.key() << "=" << it.value() << "\n";
  }
  return true;
}
