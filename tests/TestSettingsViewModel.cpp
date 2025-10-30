#include "../src/models/ConfigManager.h"
#include "../src/models/ConfigResult.h" // Include ConfigResult header
#include "../src/viewmodels/AudioViewModel.h"
#include "../src/viewmodels/InterfaceOsdViewModel.h"
#include "../src/viewmodels/PerformanceCachingViewModel.h"
#include "../src/viewmodels/PlaybackBehaviorViewModel.h"
#include "../src/viewmodels/SettingsViewModel.h"
#include "../src/viewmodels/SubtitleViewModel.h"
#include "../src/viewmodels/VideoViewModel.h"
#include <QCoreApplication>
#include <QObject>
#include <QSignalSpy>
#include <QtTest/QTest>

Q_DECLARE_METATYPE(MPVDeck::ConfigResult)

class TestSettingsViewModel : public QObject
{
  Q_OBJECT

  public:
    TestSettingsViewModel() = default;
    ~TestSettingsViewModel() = default;



  private slots:
    void init();
    void cleanup();
    void testLoadDefaults();
    void testLoadSettings();
    void testSaveSettings();
    void testRoundTrip();

  private:
    ConfigManager*     m_configManager{nullptr};
    SettingsViewModel* m_settingsViewModel{nullptr};
    QString            m_tempConfigPath;
    QTemporaryDir*     m_tempDir{nullptr};
};

void TestSettingsViewModel::init() // Used by Qt Test framework
{
  m_tempDir = new QTemporaryDir;
  if(!m_tempDir->isValid())
  {
    QFAIL("Failed to create temporary directory.");
  }
  m_tempConfigPath = QDir(m_tempDir->path()).filePath("mpv.conf");

  m_configManager     = new ConfigManager(m_tempConfigPath);
  m_settingsViewModel = new SettingsViewModel(m_configManager, nullptr);
}

void TestSettingsViewModel::cleanup() // Used by Qt Test framework
{
  delete m_settingsViewModel;
  delete m_configManager;
  delete m_tempDir;
}

void TestSettingsViewModel::testLoadDefaults() // Used by Qt Test framework
{
  m_settingsViewModel->defaultsLoader()->loadDefaults();

  // Verify some default values
  QCOMPARE(m_settingsViewModel->audioViewModel()->volume(), 100);
  QCOMPARE(m_settingsViewModel->videoViewModel()->videoProfile(),
           QString("gpu-hq"));
  QCOMPARE(m_settingsViewModel->subtitleViewModel()->subtitleFontSize(), 36);
  QCOMPARE(m_settingsViewModel->playbackBehaviorViewModel()->loopFile(),
           QString("no"));
  QCOMPARE(m_settingsViewModel->performanceCachingViewModel()->cacheSecs(), 10);
  QCOMPARE(m_settingsViewModel->interfaceOsdViewModel()->osdLevel(), 1);
}

void TestSettingsViewModel::testLoadSettings() // Used by Qt Test framework
{
  // Create a dummy config file with some values
  QString configContent = "volume=50\nprofile=low-latency\nsub-font-size=20\n";
  QFile   file(m_configManager->findConfigFile());
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
  {
    QFAIL("Failed to write to temporary config file.");
  }
  QTextStream out(&file);
  out << configContent;
  file.close();

  m_settingsViewModel->loadSettings();

  QCOMPARE(m_settingsViewModel->audioViewModel()->volume(), 50);
  QCOMPARE(m_settingsViewModel->videoViewModel()->videoProfile(),
           QString("low-latency"));
  QCOMPARE(m_settingsViewModel->subtitleViewModel()->subtitleFontSize(), 20);
}

void TestSettingsViewModel::testSaveSettings() // Used by Qt Test framework
{
  // Change some settings
  m_settingsViewModel->audioViewModel()->setVolume(75);
  m_settingsViewModel->videoViewModel()->setVideoProfile("gpu");
  m_settingsViewModel->subtitleViewModel()->setSubtitleFontSize(40);

  QSignalSpy spy(m_settingsViewModel, &SettingsViewModel::settingsSaved);

  m_settingsViewModel->saveSettings();

  QCoreApplication::processEvents(); // Process events to ensure signal is
                                     // delivered
  QCOMPARE(spy.count(), 1);          // Directly check if the signal was emitted

  // Retrieve the ConfigResult object
  MPVDeck::ConfigResult result = spy.at(0).at(0).value<MPVDeck::ConfigResult>();

  if(!result.success)
  {
    QFAIL(QString("Settings save failed: %1")
              .arg(result.errorMessage)
              .toStdString()
              .c_str());
  }

  QCOMPARE(result.success, true); // Check if settingsSaved(true) was emitted

  // Read the file directly and verify content
  QFile file(m_configManager->findConfigFile());
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QFAIL("Failed to read temporary config file after saving.");
  }
  QTextStream in(&file);
  QString     content = in.readAll();
  file.close();

  QVERIFY(content.contains("volume=75"));
  QVERIFY(content.contains("profile=gpu"));
  QVERIFY(content.contains("sub-font-size=40"));
}

void TestSettingsViewModel::testRoundTrip() // Used by Qt Test framework
{
  // 1. Load defaults
  m_settingsViewModel->defaultsLoader()->loadDefaults();

  // 2. Change some settings
  m_settingsViewModel->audioViewModel()->setVolume(150);
  m_settingsViewModel->videoViewModel()->setVideoProfile("gpu");
  m_settingsViewModel->subtitleViewModel()->setSubtitleFontSize(50);

  // 3. Save settings
  m_settingsViewModel->saveSettings();

  // 4. Create a new SettingsViewModel to simulate a fresh load
  ConfigManager*     newConfigManager = new ConfigManager(m_tempConfigPath);
  SettingsViewModel* newSettingsViewModel =
      new SettingsViewModel(newConfigManager, nullptr);

  // 5. Load settings into the new ViewModel
  newSettingsViewModel->loadSettings();

  // 6. Verify the settings in the new ViewModel
  QCOMPARE(newSettingsViewModel->audioViewModel()->volume(), 150);
  QCOMPARE(newSettingsViewModel->videoViewModel()->videoProfile(),
           QString("gpu"));
  QCOMPARE(newSettingsViewModel->subtitleViewModel()->subtitleFontSize(), 50);

  // Clean up new ViewModel and ConfigManager
  delete newSettingsViewModel;
  delete newConfigManager;
}

QTEST_MAIN(TestSettingsViewModel)
#include "TestSettingsViewModel.moc"
