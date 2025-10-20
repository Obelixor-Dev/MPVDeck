#include "../src/models/ConfigManager.h"
#include <QDir>
#include <QFile>
#include <QMetaType>
#include <QObject>
#include <QTextStream>
#include <QtTest/QTest>

using ConfigMap = QMap<QString, QString>;
Q_DECLARE_METATYPE(ConfigMap)

class TestConfigManager : public QObject {
  Q_OBJECT

private slots:
  void initTestCase();
  void testReadConfigFile_data();
  void testReadConfigFile();
  void testSaveConfigFile_data();
  void testSaveConfigFile();

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
