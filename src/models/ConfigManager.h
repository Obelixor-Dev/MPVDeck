#ifndef MPVDECK_CONFIGMANAGER_H
#define MPVDECK_CONFIGMANAGER_H

#include "ConfigResult.h"
#include <QList>
#include <QMap>
#include <QRecursiveMutex>
#include <QString>
#include <cstdint>

class ConfigManager
{
public:
  // Enum to represent the type of a line in the config file
  enum class ConfigLineType : std::uint8_t
  {
    KeyValuePair,
    Comment,
    EmptyLine,
    Invalid
  };

  // Struct to hold information about a single line in the config file
  struct ConfigLine
  {
    ConfigLineType type;
    QString        key;
    QString        value;
    QString
        rawText; // For comments and empty lines, or original key=value string
    QString errorMessage;    // New field for storing parsing errors
    QString trailingComment; // New field for storing trailing comments
  };

  static constexpr int MAX_CONFIG_LINE_LENGTH = 4096; // Define max line length

  friend class TestConfigManager; // For unit testing private methods

  explicit ConfigManager(QString configFilePath = "");
  [[nodiscard]] auto findConfigFile() const -> QString;
  [[nodiscard]] auto configFileExists() const -> bool;
  auto               readConfigFile()
      -> QList<ConfigLine>; // Returns the parsed file structure
  auto saveConfigFile(const QMap<QString, QString>& newSettings,
                      bool                          reuseExistingLines = false)
      -> MPVDeck::ConfigResult; // Updates and saves the file
  [[nodiscard]] auto getSettingsMap() const
      -> QMap<QString, QString>; // Returns current settings as a map
  [[nodiscard]] auto getRawConfig() const -> QString;
  auto parseRawConfig(const QString& configText) -> MPVDeck::ConfigResult;

  [[nodiscard]] auto getParsedConfigLines() const -> const QList<ConfigLine>&;

private:
  QString m_configFilePath;
  QList<ConfigLine>
      m_configLines;       // Stores the parsed file structure with comments
  QRecursiveMutex m_mutex; // Thread-safety for m_configLines

  // Helper to parse a single line
  static auto parseLine(const QString& line) -> ConfigLine;
  // Helper to serialize a ConfigLine back to a string
  static auto serializeLine(const ConfigLine& line) -> QString;

  // Private helper functions for saving config
  [[nodiscard]] auto validateConfigLines() const -> QStringList;
  void               updateConfigLines(QMap<QString, QString>& newSettings);
  [[nodiscard]] auto writeConfigLines(const QString& tempConfigFilePath) const
      -> MPVDeck::ConfigResult;
};

#endif // MPVDECK_CONFIGMANAGER_H