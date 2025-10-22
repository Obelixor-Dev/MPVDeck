#include "../src/models/ConfigManager.h"
#include <QDir>
#include <QFile>
#include <QMetaType>
#include <QObject>
#include <QTextStream>
#include <QtTest/QTest>

using ConfigMap = QMap<QString, QString>;
Q_DECLARE_METATYPE(ConfigMap)
Q_DECLARE_METATYPE(ConfigManager::ConfigLine)
Q_DECLARE_METATYPE(ConfigManager::ConfigLineType)

class TestConfigManager : public QObject {
  Q_OBJECT

private slots:
  void initTestCase();
  void testReadConfigFile_data();
  void testReadConfigFile();
  void testSaveConfigFile_data();
  void testSaveConfigFile();
  void testParseLine_data();
  void testParseLine();
  void testSerializeLine_data();
  void testSerializeLine();

private:
  QString m_tempConfigPath;

  void createTempConfigFile(const QString &content);
  void cleanupTempConfigFile();
};

void TestConfigManager::createTempConfigFile(const QString &content) {
  m_tempConfigPath = QDir::tempPath() + "/mpv_test_config.conf";
  QFile file(m_tempConfigPath);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    QTextStream out(&file);
    out << content;
    file.close();
  } else {
    QFAIL("Failed to create temporary config file.");
  }
}

void TestConfigManager::cleanupTempConfigFile() {
  QFile::remove(m_tempConfigPath);
}

void TestConfigManager::initTestCase() {}

void TestConfigManager::testParseLine_data() {
  QTest::addColumn<QString>("line");
  QTest::addColumn<ConfigManager::ConfigLineType>("expectedType");
  QTest::addColumn<QString>("expectedKey");
  QTest::addColumn<QString>("expectedValue");

  QTest::newRow("key=value") << "key=value" << ConfigManager::ConfigLineType::KeyValuePair << "key" << "value";
  QTest::newRow("key = value") << "key = value" << ConfigManager::ConfigLineType::KeyValuePair << "key" << "value";
  QTest::newRow("key=value # comment") << "key=value # comment" << ConfigManager::ConfigLineType::KeyValuePair << "key" << "value";
  QTest::newRow("key") << "key" << ConfigManager::ConfigLineType::KeyValuePair << "key" << "";
  QTest::newRow("key # comment") << "key # comment" << ConfigManager::ConfigLineType::KeyValuePair << "key" << "";
  QTest::newRow("# comment only") << "# comment only" << ConfigManager::ConfigLineType::Comment << "" << "";
  QTest::newRow("empty line") << "" << ConfigManager::ConfigLineType::EmptyLine << "" << "";
  QTest::newRow("key=\"value with spaces\"") << "key=\"value with spaces\"" << ConfigManager::ConfigLineType::KeyValuePair << "key" << "value with spaces";
  QTest::newRow("key=\"value with # comment\"") << "key=\"value with # comment\"" << ConfigManager::ConfigLineType::KeyValuePair << "key" << "value with # comment";
  QTest::newRow("key=value with spaces") << "key=value with spaces" << ConfigManager::ConfigLineType::KeyValuePair << "key" << "value with spaces";
}

void TestConfigManager::testParseLine() {
  QFETCH(QString, line);
  QFETCH(ConfigManager::ConfigLineType, expectedType);
  QFETCH(QString, expectedKey);
  QFETCH(QString, expectedValue);

  ConfigManager configManager; // ConfigManager instance is needed to call parseLine
  ConfigManager::ConfigLine actualLine = configManager.parseLine(line);

  QCOMPARE(actualLine.type, expectedType);
  QCOMPARE(actualLine.key, expectedKey);
  QCOMPARE(actualLine.value, expectedValue);
}

void TestConfigManager::testSerializeLine_data() {
  QTest::addColumn<ConfigManager::ConfigLine>("line");
  QTest::addColumn<QString>("expectedLine");

  ConfigManager::ConfigLine line1 = {ConfigManager::ConfigLineType::KeyValuePair, "key", "value", ""};
  QTest::newRow("key=value") << line1 << "key=value";

  ConfigManager::ConfigLine line2 = {ConfigManager::ConfigLineType::KeyValuePair, "key", "", ""};
  QTest::newRow("key") << line2 << "key";

  ConfigManager::ConfigLine line3 = {ConfigManager::ConfigLineType::KeyValuePair, "key", "value with spaces", ""};
  QTest::newRow("key=value with spaces") << line3 << "key=\"value with spaces\"";

  ConfigManager::ConfigLine line4 = {ConfigManager::ConfigLineType::KeyValuePair, "key", "value with \"quotes\"", ""};
  QTest::newRow("key=\"value with quotes\"") << line4 << "key=\"value with \"quotes\"\"";

  ConfigManager::ConfigLine line5 = {ConfigManager::ConfigLineType::KeyValuePair, "key", "value with # comment", ""};
  QTest::newRow("key=value with # comment") << line5 << "key=\"value with # comment\"";

  ConfigManager::ConfigLine line6 = {ConfigManager::ConfigLineType::Comment, "", "", "# a comment"};
  QTest::newRow("comment only") << line6 << "# a comment";

  ConfigManager::ConfigLine line7 = {ConfigManager::ConfigLineType::EmptyLine, "", "", ""};
  QTest::newRow("empty line") << line7 << "";
}

void TestConfigManager::testSerializeLine() {
  QFETCH(ConfigManager::ConfigLine, line);
  QFETCH(QString, expectedLine);

  ConfigManager configManager; // ConfigManager instance is needed to call serializeLine
  QString actualLine = configManager.serializeLine(line);

  QCOMPARE(actualLine, expectedLine);
}

void TestConfigManager::testReadConfigFile_data() {
  QTest::addColumn<QString>("fileContent");
  QTest::addColumn<ConfigMap>("expectedSettings");

  ConfigMap settings1;
  settings1["volume"] = "100";
  settings1["mute"] = "yes";
  QTest::newRow("basic settings") << "volume=100\nmute=yes\n" << settings1;

  ConfigMap settings2;
  settings2["vo"] = "gpu";
  settings2["profile"] = "gpu-hq";
  QTest::newRow("video settings") << "vo=gpu\nprofile=gpu-hq\n" << settings2;

  ConfigMap settings3;
  settings3["flag"] = "";
  QTest::newRow("flag setting") << "flag\n" << settings3;

  ConfigMap settings4;
  settings4["key"] = "value with spaces";
  QTest::newRow("value with spaces") << "key=value with spaces\n" << settings4;

  ConfigMap settings5;
  settings5["key"] = "value with";
  QTest::newRow("value with inline comment")
      << "key=value with # comment # inline comment\n"
      << settings5;

  ConfigMap settings6;
  settings6["key"] = "value with quotes";
  QTest::newRow("value with quotes") << "key=\"value with quotes\"\n"
                                     << settings6;

  ConfigMap settings7;
  settings7["key"] = "value with # comment";
  QTest::newRow("value with inline comment and quotes")
      << "key=\"value with # comment\" # inline comment\n"
      << settings7;

  ConfigMap settings8;
  QTest::newRow("empty file") << "" << settings8;

  ConfigMap settings9;
  QTest::newRow("comments only") << "# comment1\n# comment2\n" << settings9;
}

void TestConfigManager::testReadConfigFile() {
  QFETCH(QString, fileContent);
  QFETCH(ConfigMap, expectedSettings);

  createTempConfigFile(fileContent);
  ConfigManager configManager(m_tempConfigPath);

  configManager.readConfigFile();
  QMap<QString, QString> actualSettings = configManager.getSettingsMap();

  QCOMPARE(actualSettings.size(), expectedSettings.size());
  for (auto it = expectedSettings.constBegin();
       it != expectedSettings.constEnd(); ++it) {
    QVERIFY(actualSettings.contains(it.key()));
    QCOMPARE(actualSettings.value(it.key()), it.value());
  }

  cleanupTempConfigFile();
}

void TestConfigManager::testSaveConfigFile_data() {
  QTest::addColumn<ConfigMap>("settingsToSave");
  QTest::addColumn<QString>("expectedFileContent");

  ConfigMap settings1;
  settings1["volume"] = "100";
  settings1["mute"] = "yes";
  QTest::newRow("basic settings") << settings1 << "volume=100\nmute=yes\n";

  ConfigMap settings2;
  settings2["vo"] = "gpu";
  settings2["profile"] = "gpu-hq";
  QTest::newRow("video settings") << settings2 << "vo=gpu\nprofile=gpu-hq\n";

  ConfigMap settings3;
  settings3["flag"] = "";
  QTest::newRow("flag setting") << settings3 << "flag=\n";

  ConfigMap settings4;
  settings4["key"] = "value with spaces";
  QTest::newRow("value with spaces") << settings4 << "key=value with spaces\n";

  ConfigMap settings5;
  settings5["key"] = "value with \"quotes\"";
  QTest::newRow("value with quotes")
      << settings5 << "key=value with \"quotes\"\n";
}

void TestConfigManager::testSaveConfigFile() {
  QFETCH(ConfigMap, settingsToSave);
  QFETCH(QString, expectedFileContent);

  createTempConfigFile(""); // Create an empty file to be overwritten
  ConfigManager configManager(m_tempConfigPath);

  // Call saveConfigFile
  configManager.saveConfigFile(settingsToSave);

  // Read the content back using ConfigManager::readConfigFile and compare maps
  configManager.readConfigFile();
  ConfigMap actualSettings = configManager.getSettingsMap();

  QCOMPARE(actualSettings.size(), settingsToSave.size());
  for (auto it = settingsToSave.constBegin(); it != settingsToSave.constEnd();
       ++it) {
    QVERIFY(actualSettings.contains(it.key()));
    QCOMPARE(actualSettings.value(it.key()), it.value());
  }

  cleanupTempConfigFile();
}

QTEST_MAIN(TestConfigManager)
#include "TestConfigManager.moc"
