#ifndef MPVDECK_CONFIGMANAGER_H
#define MPVDECK_CONFIGMANAGER_H

#include "ConfigResult.h"
#include <QList>
#include <QMap>
#include <QString>
#include <cstdint>

class ConfigManager {
public:
  // Enum to represent the type of a line in the config file
  enum class ConfigLineType : std::uint8_t { KeyValuePair, Comment, EmptyLine };

  // Struct to hold information about a single line in the config file
  struct ConfigLine {
    ConfigLineType type;
    QString key;
    QString value;
    QString
        rawText; // For comments and empty lines, or original key=value string
  };

  friend class TestConfigManager; // For unit testing private methods

  explicit ConfigManager(QString configFilePath = "");
  [[nodiscard]] auto findConfigFile() const -> QString;
  [[nodiscard]] auto configFileExists() const -> bool;
  auto readConfigFile()
      -> QList<ConfigLine>; // Returns the parsed file structure
  auto saveConfigFile(const QMap<QString, QString> &newSettings,
                      bool reuseExistingLines = false)
      -> MPVDeck::ConfigResult; // Updates and saves the file
  [[nodiscard]] auto getSettingsMap() const
      -> QMap<QString, QString>; // Returns current settings as a map
  [[nodiscard]] auto getRawConfig() const -> QString;
  void parseRawConfig(const QString &configText);

private:
  QString m_configFilePath;
  QList<ConfigLine>
      m_configLines; // Stores the parsed file structure with comments

  // Helper to parse a single line
  static auto parseLine(const QString &line) -> ConfigLine;
  // Helper to serialize a ConfigLine back to a string
  static auto serializeLine(const ConfigLine &line) -> QString;
};

#endif // MPVDECK_CONFIGMANAGER_H