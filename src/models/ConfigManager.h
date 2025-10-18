#ifndef MPVDECK_CONFIGMANAGER_H
#define MPVDECK_CONFIGMANAGER_H

#include <QString>
#include <QMap>

class ConfigManager {
public:
    ConfigManager();

    QString findConfigFile();
    QMap<QString, QString> readConfigFile();
    void saveConfigFile(const QMap<QString, QString> &settings);

private:
    QString m_configFilePath;
};

#endif //MPVDECK_CONFIGMANAGER_H
