#include "../src/models/ConfigManager.h"
#include "../src/viewmodels/DefaultsLoader.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaType>
#include <QObject>
#include <QTextStream>
#include <QtTest/QTest>

using ConfigMap = QMap<QString, QString>;
Q_DECLARE_METATYPE(ConfigMap)
Q_DECLARE_METATYPE(ConfigManager::ConfigLine)
Q_DECLARE_METATYPE(ConfigManager::ConfigLineType)

class TestConfigManager : public QObject
{
  Q_OBJECT

  public:
    TestConfigManager()  = default;
    ~TestConfigManager() = default;



  private slots:
    void        initTestCase();
    static void testReadConfigFile_data();
    void        testReadConfigFile();
    static void testSaveConfigFile_data();
    void        testSaveConfigFile();
    static void testParseLine_data();
    void        testParseLine();
    static void testSerializeLine_data();
    void        testSerializeLine();
    static void testParseRawConfig_data();
    void        testParseRawConfig();
    void        cleanupTestCase();

  private:
    QString        m_tempConfigPath;
    QTemporaryDir* m_tempDir{nullptr};

    void createTempConfigFile(const QString& content);
};

void TestConfigManager::createTempConfigFile(const QString& content)
{
  QFile file(m_tempConfigPath);
  if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
  {
    QTextStream out(&file);
    out << content;
    file.close();
  }
  else
  {
    QFAIL("Failed to create temporary config file.");
  }
}

void TestConfigManager::cleanupTestCase()
{ // Used by Qt Test framework
  delete m_tempDir;
}

void TestConfigManager::initTestCase()
{ // Used by Qt Test framework
  m_tempDir = new QTemporaryDir;
  if(!m_tempDir->isValid())
  {
    QFAIL("Failed to create temporary directory.");
  }
  m_tempConfigPath = QDir(m_tempDir->path()).filePath("mpv_test_config.conf");
}

void TestConfigManager::testParseLine_data()
{ // Used by Qt Test framework
  QTest::addColumn<QString>("line");
  QTest::addColumn<ConfigManager::ConfigLineType>("expectedType");
  QTest::addColumn<QString>("expectedKey");
  QTest::addColumn<QString>("expectedValue");
  QTest::addColumn<QString>("expectedTrailingComment");
  QTest::addColumn<QString>("expectedErrorMessage");

  // Valid Key-Value Pairs
  QTest::newRow("key_value")
      << "key=value" << ConfigManager::ConfigLineType::KeyValuePair << "key"
      << "value" << "" << "";
  QTest::newRow("key_space_value")
      << "key = value" << ConfigManager::ConfigLineType::KeyValuePair << "key"
      << "value" << "" << "";
  QTest::newRow("key_value_comment")
      << "key=value # comment" << ConfigManager::ConfigLineType::KeyValuePair
      << "key" << "value" << "# comment" << "";
  QTest::newRow("key_value_with_spaces")
      << "key=value with spaces" << ConfigManager::ConfigLineType::KeyValuePair
      << "key" << "value with spaces" << "" << "";
  QTest::newRow("key_quoted_value_with_spaces")
      << R"(key="value with spaces")"
      << ConfigManager::ConfigLineType::KeyValuePair << "key"
      << "value with spaces" << "" << "";
  QTest::newRow("key_quoted_value_with_comment")
      << R"(key="value with # comment")"
      << ConfigManager::ConfigLineType::KeyValuePair << "key"
      << "value with # comment" << "" << "";
  QTest::newRow("key_quoted_value_with_escaped_quotes")
      << R"(key="value with \"escaped quotes\"")"
      << ConfigManager::ConfigLineType::KeyValuePair << "key"
      << "value with \"escaped quotes\"" << "" << "";
  QTest::newRow("key_quoted_value_with_backslashes")
      << R"(key="value with \\backslashes\\")"
      << ConfigManager::ConfigLineType::KeyValuePair << "key"
      << "value with \\backslashes\\" << "" << "";
  QTest::newRow("key_quoted_value_with_escaped_quotes_and_comment")
      << R"(key="value with \"quotes\" # inline comment")"
      << ConfigManager::ConfigLineType::KeyValuePair << "key"
      << "value with \"quotes\" # inline comment" << "" << "";
  QTest::newRow("key_value_with_inline_comment_chars")
      << "key=value with # comment # and another"
      << ConfigManager::ConfigLineType::KeyValuePair << "key" << "value with"
      << "# comment # and another" << "";

  // Flags (key with no value)
  QTest::newRow("key_flag")
      << "key" << ConfigManager::ConfigLineType::KeyValuePair << "key" << ""
      << "" << "";
  QTest::newRow("key_flag_comment")
      << "key # comment" << ConfigManager::ConfigLineType::KeyValuePair << "key"
      << "" << "# comment" << "";
  QTest::newRow("flag_with_dashes")
      << "flag-with-dashes" << ConfigManager::ConfigLineType::KeyValuePair
      << "flag-with-dashes" << "" << "" << "";
  QTest::newRow("flag_with_spaces")
      << "flag with spaces" << ConfigManager::ConfigLineType::KeyValuePair
      << "flag with spaces" << "" << "" << "";

  // Comments
  QTest::newRow("comment_only")
      << "# comment only" << ConfigManager::ConfigLineType::Comment << "" << ""
      << "" << "";
  QTest::newRow("indented_comment")
      << "  # indented comment" << ConfigManager::ConfigLineType::Comment << ""
      << "" << "" << "";

  // Empty Lines
  QTest::newRow("empty_line")
      << "" << ConfigManager::ConfigLineType::EmptyLine << "" << "" << "" << "";
  QTest::newRow("whitespace_only_line")
      << "  " << ConfigManager::ConfigLineType::EmptyLine << "" << "" << ""
      << ""; // Whitespace only

  // Invalid Lines
  QTest::newRow("invalid_empty_key")
      << "=value" << ConfigManager::ConfigLineType::Invalid << "" << "" << ""
      << "Key cannot be empty in a key-value pair.";
  QTest::newRow("invalid_indented_empty_key")
      << "  =value" << ConfigManager::ConfigLineType::Invalid << "" << "" << ""
      << "Key cannot be empty in a key-value pair.";
  QTest::newRow("invalid_quoted_key")
      << "\"quoted key\"=value" << ConfigManager::ConfigLineType::KeyValuePair
      << "\"quoted key\"" << "value" << ""
      << ""; // Current implementation treats quoted key as a normal key.
  QTest::newRow("just_a_value")
      << "just a value" << ConfigManager::ConfigLineType::KeyValuePair
      << "just a value" << "" << "" << ""; // Treated as a flag
}

void TestConfigManager::testParseLine()
{ // Used by Qt Test framework
  QFETCH(QString, line);
  QFETCH(ConfigManager::ConfigLineType, expectedType);
  QFETCH(QString, expectedKey);
  QFETCH(QString, expectedValue);
  QFETCH(QString, expectedTrailingComment);
  QFETCH(QString, expectedErrorMessage);

  ConfigManager::ConfigLine actualLine = ConfigManager::parseLine(line);

  QCOMPARE(actualLine.type, expectedType);
  QCOMPARE(actualLine.key, expectedKey);
  QCOMPARE(actualLine.value, expectedValue);
  QCOMPARE(actualLine.trailingComment, expectedTrailingComment);
  QCOMPARE(actualLine.errorMessage, expectedErrorMessage);
}

void TestConfigManager::testSerializeLine_data()
{ // Used by Qt Test framework
  QTest::addColumn<ConfigManager::ConfigLine>("line");
  QTest::addColumn<QString>("expectedLine");

  // Key-Value Pairs
  QTest::newRow("key_value")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::KeyValuePair,
                                   .key = "key",
                                   .value = "value"}
      << "key=value";
  QTest::newRow("key_flag")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::KeyValuePair,
                                   .key = "key"}
      << "key=";
  QTest::newRow("key_value_with_spaces")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::KeyValuePair,
                                   .key = "key",
                                   .value = "value with spaces"}
      << R"(key="value with spaces")";
  QTest::newRow("key_value_with_comment_char")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::KeyValuePair,
                                   .key = "key",
                                   .value = "value with # comment"}
      << R"(key="value with # comment")";
  QTest::newRow("key_value_with_equals_sign")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::KeyValuePair,
                                   .key = "key",
                                   .value = "value with = sign"}
      << "key=\"value with = sign\"";
  QTest::newRow("key_value_with_quotes")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::KeyValuePair,
                                   .key = "key",
                                   .value = "value with \"quotes\""}
      << R"(key="value with \"quotes\"")";
  QTest::newRow("key_value_with_backslashes")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::KeyValuePair,
                                   .key = "key",
                                   .value = "value with \\backslashes\\"}
      << R"(key="value with \\backslashes\\")";
  QTest::newRow("key_value_with_trailing_comment")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::KeyValuePair,
                                   .key = "key",
                                   .value = "value",
                                   .trailingComment = " # comment"}
      << "key=value # comment";
  QTest::newRow("key_value_with_spaces_and_trailing_comment")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::KeyValuePair,
                                   .key = "key",
                                   .value = "value with spaces",
                                   .trailingComment = " # comment"}
      << "key=\"value with spaces\" # comment";

  // Comments
  QTest::newRow("comment_only")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::Comment,
                                   .rawText = "# a comment"}
      << "# a comment";
  QTest::newRow("indented_comment")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::Comment,
                                   .rawText = "  # a comment"}
      << "  # a comment";

  // Empty Lines
  QTest::newRow("empty_line")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::EmptyLine}
      << "";
  QTest::newRow("whitespace_only_line")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::EmptyLine,
                                   .rawText = "  "}
      << "  ";

  // Invalid Lines
  QTest::newRow("invalid_line_empty_key")
      << ConfigManager::ConfigLine{.type = ConfigManager::ConfigLineType::Invalid,
                                   .rawText = "=value",
                                   .errorMessage = "Key cannot be empty in a key-value pair."}
      << "# ERROR: Key cannot be empty in a key-value pair. -- Original: "
         "=value";
}

void TestConfigManager::testSerializeLine()
{ // Used by Qt Test framework
  QFETCH(ConfigManager::ConfigLine, line);
  QFETCH(QString, expectedLine);

  QString actualLine = ConfigManager::serializeLine(line);

  QCOMPARE(actualLine, expectedLine);
}

void TestConfigManager::testParseRawConfig_data()
{ // Used by Qt Test framework
  QTest::addColumn<QString>("input");
  QTest::addColumn<bool>("expectedSuccess");
  QTest::addColumn<QString>("expectedErrorMessage");

  QTest::newRow("valid_config") << "volume=100\nmute=yes" << true << "";
  QTest::newRow("invalid_config_empty_key")
      << "=invalid\nkey=value" << false
      << "Invalid lines detected:\nInvalid line: '=invalid' - Key cannot be "
         "empty in a key-value pair.";
  QTest::newRow("invalid_config_oversized_line")
      << "key=" + QString(ConfigManager::MAX_CONFIG_LINE_LENGTH + 1, 'a')
      << false
      << "Invalid lines detected:\nInvalid line: 'key=" +
             QString(ConfigManager::MAX_CONFIG_LINE_LENGTH - 4, 'a') +
             "...' - Line exceeds maximum allowed length of " +
             QString::number(ConfigManager::MAX_CONFIG_LINE_LENGTH) +
             " characters.";
  QTest::newRow("mixed_valid_and_invalid")
      << "valid_key=value\n=invalid\nanother_valid=another_value" << false
      << "Invalid lines detected:\nInvalid line: '=invalid' - Key cannot be "
         "empty in a key-value pair.";
}

void TestConfigManager::testParseRawConfig()
{ // Used by Qt Test framework
  QFETCH(QString, input);
  QFETCH(bool, expectedSuccess);
  QFETCH(QString, expectedErrorMessage);

  ConfigManager         configManager(m_tempConfigPath); // Use a temporary path
  MPVDeck::ConfigResult result = configManager.parseRawConfig(input);

  QCOMPARE(result.success, expectedSuccess);
  QCOMPARE(result.errorMessage, expectedErrorMessage);

  // If successful, verify the settings map
  if(expectedSuccess)
  {
    QMap<QString, QString> expectedSettings;
    QStringList            lines = input.split('\n');
    for(const QString& line : lines)
    {
      ConfigManager::ConfigLine parsedLine = configManager.parseLine(line);
      if(parsedLine.type == ConfigManager::ConfigLineType::KeyValuePair)
      {
        expectedSettings[parsedLine.key] = parsedLine.value;
      }
    }
    QCOMPARE(configManager.getSettingsMap(), expectedSettings);
  }
}

void TestConfigManager::testReadConfigFile_data()
{ // Used by Qt Test framework
  QTest::addColumn<QString>("fileContent");
  QTest::addColumn<ConfigMap>("expectedSettings");

  ConfigMap settings1;
  settings1["volume"] = "100";
  settings1["mute"]   = "yes";
  QTest::newRow("basic_settings") << "volume=100\nmute=yes\n" << settings1;

  ConfigMap settings2;
  settings2["vo"]      = "gpu";
  settings2["profile"] = "gpu-hq";
  QTest::newRow("video_settings") << "vo=gpu\nprofile=gpu-hq\n" << settings2;

  ConfigMap settings3;
  settings3["flag"] = "";
  QTest::newRow("flag_setting") << "flag\n" << settings3;

  ConfigMap settings4;
  settings4["key"] = "value with spaces";
  QTest::newRow("value_with_spaces") << "key=value with spaces\n" << settings4;

  ConfigMap settings5;
  settings5["key"] = "value with";
  QTest::newRow("value_with_inline_comment")
      << "key=value with # comment # inline comment\n"
      << settings5;

  ConfigMap settings6;
  settings6["key"] = "value with quotes";
  QTest::newRow("value_with_quotes") << "key=\"value with quotes\"\n"
                                     << settings6;

  ConfigMap settings7;
  settings7["key"] = "value with # comment";
  QTest::newRow("value_with_inline_comment_and_quotes")
      << "key=\"value with # comment\" # inline comment\n"
      << settings7;

  ConfigMap settings8;
  QTest::newRow("empty_file") << "" << settings8;

  QTest::newRow("comments_only") << "# comment1\n# comment2\n" << ConfigMap();

  // New test cases
  ConfigMap settings10;
  settings10["key1"] = "value1";
  settings10["key2"] = "value2";
  QTest::newRow("mixed_content")
      << "key1=value1\n# This is a comment\n\nkey2=value2 # inline comment\n"
      << settings10;

  ConfigMap settings11;
  settings11["path"] = "C:\\Program Files\\mpv";
  QTest::newRow("value_with_escaped_backslashes")
      << R"(path="C:\\Program Files\\mpv")" << settings11;

  ConfigMap settings12;
  settings12["title"] = "Movie with \"quotes\"";
  QTest::newRow("value_with_escaped_quotes")
      << R"(title="Movie with \"quotes\"")" << settings12;

  ConfigMap settings13;
  settings13["option"] = "value";
  QTest::newRow("line_with_only_trailing_comment")
      << "option=value # This is a trailing comment\n"
      << settings13;

  ConfigMap settings14;
  settings14["key"] = "";
  QTest::newRow("flag_with_trailing_comment") << "key # trailing comment\n"
                                              << settings14;

  ConfigMap settings15;
  QString   longLine =
      "key=" + QString(ConfigManager::MAX_CONFIG_LINE_LENGTH + 1, 'a');
  QTest::newRow("oversized_line") << longLine + "\n" << settings15;
}

void TestConfigManager::testReadConfigFile()
{ // Used by Qt Test framework
  QFETCH(QString, fileContent);
  QFETCH(ConfigMap, expectedSettings);

  createTempConfigFile(fileContent);
  ConfigManager configManager(m_tempConfigPath);

  configManager.readConfigFile();
  QMap<QString, QString> actualSettings = configManager.getSettingsMap();

  if(QString(QTest::currentTestFunction()) ==
     "testReadConfigFile/oversized_line")
  {
    QVERIFY(actualSettings.isEmpty());
    const QList<ConfigManager::ConfigLine>& parsedLines =
        configManager.getParsedConfigLines();
    QVERIFY(!parsedLines.isEmpty());
    QCOMPARE(parsedLines.last().type, ConfigManager::ConfigLineType::Invalid);
    QVERIFY(parsedLines.last().errorMessage.contains(
        "Line exceeds maximum allowed length"));
  }
  else
  {
    QCOMPARE(actualSettings.size(), expectedSettings.size());
    for(auto it = expectedSettings.constBegin();
        it != expectedSettings.constEnd(); ++it)
    {
      QVERIFY(actualSettings.contains(it.key()));
      QCOMPARE(actualSettings.value(it.key()), it.value());
    }
  }
}

void TestConfigManager::testSaveConfigFile_data()
{ // Used by Qt Test framework
  QTest::addColumn<ConfigMap>("settingsToSave");
  QTest::addColumn<QString>("initialFileContent");
  QTest::addColumn<QString>("expectedFileContentAfterSave");
  QTest::addColumn<QString>("expectedSaveResultErrorMessage");
  QTest::addColumn<bool>("reuseExistingLines");

  ConfigMap settings1;
  settings1["volume"] = "100";
  settings1["mute"]   = "yes";
  QTest::newRow("basic_settings") << settings1 << "" << "mute=yes\nvolume=100\n"
                                  << "" << false;

  ConfigMap settings2;
  settings2["vo"]      = "gpu";
  settings2["profile"] = "gpu-hq";
  QTest::newRow("video_settings")
      << settings2 << "" << "profile=gpu-hq\nvo=gpu\n"
      << "" << false;

  ConfigMap settings3;
  settings3["flag"] = "";
  QTest::newRow("flag_setting") << settings3 << "" << "flag=\n" << "" << false;

  ConfigMap settings4;
  settings4["key"] = "value with spaces";
  QTest::newRow("value_with_spaces")
      << settings4 << "" << "key=\"value with spaces\"\n"
      << "" << false;

  ConfigMap settings5;
  settings5["key"] = "value with \"quotes\"";
  QTest::newRow("value_with_quotes")
      << settings5 << "" << "key=\"value with \\\"quotes\\\"\"\n"
      << "" << false;

  ConfigMap settings6;
  settings6["path"] = "C:\\Program Files\\mpv";
  QTest::newRow("value_with_backslashes")
      << settings6 << "" << "path=\"C:\\\\Program Files\\\\mpv\"\n"
      << "" << false;

  ConfigMap settings7;
  settings7["key"] = "value with # comment";
  QTest::newRow("value_with_hash")
      << settings7 << "" << "key=\"value with # comment\"\n"
      << "" << false;

  ConfigMap settings8;
  settings8["key"] = "value with = sign";
  QTest::newRow("value_with_equals_sign")
      << settings8 << "" << "key=\"value with = sign\"\n"
      << "" << false;

  ConfigMap settings9;
  QTest::newRow("empty_map") << ConfigMap() << "" << "" << "" << false;

  ConfigMap settings10;
  settings10["valid_key"] = "valid_value";
  QTest::newRow("invalid_line_in_config_lines")
      << settings10 << "=invalid\nvalid_key=valid_value\n"
      << ""
      << "Cannot save configuration due to invalid lines:\nInvalid line: "
         "'=invalid' - Key cannot be empty in a key-value pair."
      << true;

  QTest::newRow("multiple_invalid_lines")
      << ConfigMap{{"key", "value"}} << "=invalid1\n=invalid2\n"
      << ""
      << "Cannot save configuration due to invalid lines:\nInvalid line: "
         "'=invalid1' - Key cannot be empty in a key-value pair.\nInvalid "
         "line: '=invalid2' - Key cannot be empty in a key-value pair."
      << true;
}

void TestConfigManager::testSaveConfigFile()
{ // Used by Qt Test framework
  QFETCH(ConfigMap, settingsToSave);
  QFETCH(QString, initialFileContent);
  QFETCH(QString, expectedFileContentAfterSave);
  QFETCH(QString, expectedSaveResultErrorMessage);
  QFETCH(bool, reuseExistingLines);

  createTempConfigFile(initialFileContent);
  ConfigManager configManager(m_tempConfigPath);

  // Call saveConfigFile
  MPVDeck::ConfigResult result =
      configManager.saveConfigFile(settingsToSave, reuseExistingLines);

  if(expectedSaveResultErrorMessage.isEmpty())
  {
    QVERIFY(result.success);

    // Read the content back from the file and compare with
    // expectedFileContentAfterSave
    QFile file(m_tempConfigPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QFAIL("Failed to open saved config file for reading.");
    }
    QTextStream in(&file);
    QString     actualFileContent = in.readAll();
    file.close();

    QCOMPARE(actualFileContent, expectedFileContentAfterSave);

    // Also verify the settings map is correct after a successful save
    configManager.readConfigFile();
    QMap<QString, QString> actualSettings = configManager.getSettingsMap();

    QCOMPARE(actualSettings.size(), settingsToSave.size());
    for(auto it = settingsToSave.constBegin(); it != settingsToSave.constEnd();
        ++it)
    {
      QVERIFY(actualSettings.contains(it.key()));
      QCOMPARE(actualSettings.value(it.key()), it.value());
    }
  }
  else
  {
    QVERIFY(!result.success);
    QCOMPARE(result.errorMessage, expectedSaveResultErrorMessage);
  }
}

QTEST_MAIN(TestConfigManager)
#include "TestConfigManager.moc"