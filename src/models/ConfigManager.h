#ifndef MPVDECK_CONFIGMANAGER_H
#define MPVDECK_CONFIGMANAGER_H

#include <QMap>
#include <QString>

class ConfigManager {
public:
  static auto findConfigFile() -> QString;
  static auto readConfigFile() -> QMap<QString, QString>;
  static auto saveConfigFile(const QMap<QString, QString> &settings) -> bool;
};

#endif // MPVDECK_CONFIGMANAGER_H