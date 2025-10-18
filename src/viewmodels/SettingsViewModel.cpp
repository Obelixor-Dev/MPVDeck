#include "SettingsViewModel.h"
#include "../models/ConfigManager.h"

SettingsViewModel::SettingsViewModel(QObject *parent) : QObject(parent) {
}

void SettingsViewModel::loadSettings() {
    ConfigManager configManager;
    m_settings = configManager.readConfigFile();
}

QMap<QString, QString> SettingsViewModel::settings() const {
    return m_settings;
}
