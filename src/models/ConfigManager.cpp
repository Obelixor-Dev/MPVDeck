#include "ConfigManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTextStream>

ConfigManager::ConfigManager() {
    m_configFilePath = findConfigFile();
}

QString ConfigManager::findConfigFile() {
    // Get the standard config location using Qt's helper functions
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    if (configPath.isEmpty()) {
        // Fallback to the home directory if XDG_CONFIG_HOME is not set
        configPath = QDir::homePath() + "/.config";
    }

    QDir mpvConfigDir(configPath + "/mpv");

    if (mpvConfigDir.exists("mpv.conf")) {
        return mpvConfigDir.filePath("mpv.conf");
    }

    // Return an empty string if not found
    return QString();
}

QMap<QString, QString> ConfigManager::readConfigFile() {
    QMap<QString, QString> settings;
    if (m_configFilePath.isEmpty()) {
        qWarning() << "Config file path is not set.";
        return settings;
    }

    QFile configFile(m_configFilePath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open config file:" << m_configFilePath;
        return settings;
    }

    QTextStream in(&configFile);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        // Ignore comments and empty lines
        if (line.isEmpty() || line.startsWith('#')) {
            continue;
        }

        // Strip inline comments
        int commentIndex = line.indexOf('#');
        if (commentIndex != -1) {
            line = line.left(commentIndex).trimmed();
        }

        if (line.isEmpty()) {
            continue;
        }

        int separatorIndex = line.indexOf('=');
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
