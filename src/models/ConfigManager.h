#ifndef MPVDECK_CONFIGMANAGER_H
#define MPVDECK_CONFIGMANAGER_H

#include <QMap>
#include <QString>
#include <QList>

class ConfigManager {
public:
  // Enum to represent the type of a line in the config file
  enum ConfigLineType {
    KeyValuePair,
    Comment,
    EmptyLine
  };

  // Struct to hold information about a single line in the config file
  struct ConfigLine {
    ConfigLineType type;
    QString key;
    QString value;
    QString rawText; // For comments and empty lines, or original key=value string
  };

  explicit ConfigManager(const QString& configFilePath = "");
  auto findConfigFile() -> QString;
  auto readConfigFile() -> QList<ConfigLine>; // Returns the parsed file structure
  auto saveConfigFile(const QMap<QString, QString> &newSettings) -> bool; // Updates and saves the file
  auto getSettingsMap() const -> QMap<QString, QString>; // Returns current settings as a map
  QString getRawConfig() const;
  void parseRawConfig(const QString &configText);

private:
  QString m_configFilePath;
  QList<ConfigLine> m_configLines; // Stores the parsed file structure with comments

  // Helper to parse a single line
  auto parseLine(const QString& line) -> ConfigLine;
  // Helper to serialize a ConfigLine back to a string
  auto serializeLine(const ConfigLine& line) const -> QString;
};

#endif // MPVDECK_CONFIGMANAGER_H