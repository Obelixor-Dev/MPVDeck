#include <QCoreApplication>
#include <QtTest/QTest>
#include <QObject>
#include <QSignalSpy>
#include "../src/viewmodels/SettingsViewModel.h"
#include "../src/models/ConfigManager.h"
#include "../src/viewmodels/AudioViewModel.h"
#include "../src/viewmodels/VideoViewModel.h"
#include "../src/viewmodels/SubtitleViewModel.h"
#include "../src/viewmodels/PlaybackBehaviorViewModel.h"
#include "../src/viewmodels/PerformanceCachingViewModel.h"
#include "../src/viewmodels/InterfaceOsdViewModel.h"

class TestSettingsViewModel : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testLoadDefaults();
    void testLoadSettings();
    void testSaveSettings();
    void testRoundTrip();

private:
    ConfigManager* m_configManager;
    SettingsViewModel* m_settingsViewModel;
    QString m_tempConfigPath;
};

void TestSettingsViewModel::init()
{
    // Create a temporary config file for testing
    m_tempConfigPath = QDir::tempPath() + "/mpv_test_config.conf";
    QFile file(m_tempConfigPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QFAIL("Failed to create temporary config file.");
    }
    file.close();

    m_configManager = new ConfigManager(m_tempConfigPath);
    m_settingsViewModel = new SettingsViewModel(m_configManager, nullptr);
}

void TestSettingsViewModel::cleanup()
{
    delete m_settingsViewModel;
    delete m_configManager;
    QFile::remove(m_tempConfigPath);
}

void TestSettingsViewModel::testLoadDefaults()
{
    m_settingsViewModel->loadDefaults();

    // Verify some default values
    QCOMPARE(m_settingsViewModel->audioViewModel()->volume(), 100);
    QCOMPARE(m_settingsViewModel->videoViewModel()->videoProfile(), QString("gpu-hq"));
    QCOMPARE(m_settingsViewModel->subtitleViewModel()->subtitleFontSize(), 36);
    QCOMPARE(m_settingsViewModel->playbackBehaviorViewModel()->loopFile(), QString("no"));
    QCOMPARE(m_settingsViewModel->performanceCachingViewModel()->cacheSecs(), 10);
    QCOMPARE(m_settingsViewModel->interfaceOsdViewModel()->osdLevel(), 1);
}

void TestSettingsViewModel::testLoadSettings()
{
    // Create a dummy config file with some values
    QString configContent = "volume=50\nprofile=low-latency\nsub-font-size=20\n";
    QFile file(m_configManager->findConfigFile());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QFAIL("Failed to write to temporary config file.");
    }
    QTextStream out(&file);
    out << configContent;
    file.close();

    m_settingsViewModel->loadSettings();

    QCOMPARE(m_settingsViewModel->audioViewModel()->volume(), 50);
    QCOMPARE(m_settingsViewModel->videoViewModel()->videoProfile(), QString("low-latency"));
    QCOMPARE(m_settingsViewModel->subtitleViewModel()->subtitleFontSize(), 20);
}

void TestSettingsViewModel::testSaveSettings()
{
    // Change some settings
    m_settingsViewModel->audioViewModel()->setVolume(75);
    m_settingsViewModel->videoViewModel()->setVideoProfile("gpu");
    m_settingsViewModel->subtitleViewModel()->setSubtitleFontSize(40);

    QSignalSpy spy(m_settingsViewModel, &SettingsViewModel::settingsSaved);

    m_settingsViewModel->saveSettings();

    QCoreApplication::processEvents(); // Process events to ensure signal is delivered
    QCOMPARE(spy.count(), 1); // Directly check if the signal was emitted
    QCOMPARE(spy.at(0).at(0).toBool(), true); // Check if settingsSaved(true) was emitted

    // Read the file directly and verify content
    QFile file(m_configManager->findConfigFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QFAIL("Failed to read temporary config file after saving.");
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QVERIFY(content.contains("volume=75"));
    QVERIFY(content.contains("profile=gpu"));
    QVERIFY(content.contains("sub-font-size=40"));
}

void TestSettingsViewModel::testRoundTrip()
{
    // 1. Load defaults
    m_settingsViewModel->loadDefaults();

    // 2. Change some settings
    m_settingsViewModel->audioViewModel()->setVolume(150);
    m_settingsViewModel->videoViewModel()->setVideoProfile("high-quality");
    m_settingsViewModel->subtitleViewModel()->setSubtitleFontSize(50);

    // 3. Save settings
    m_settingsViewModel->saveSettings();

    // 4. Create a new SettingsViewModel to simulate a fresh load
    ConfigManager* newConfigManager = new ConfigManager(m_tempConfigPath);
    SettingsViewModel* newSettingsViewModel = new SettingsViewModel(newConfigManager, nullptr);

    // 5. Load settings into the new ViewModel
    newSettingsViewModel->loadSettings();

    // 6. Verify the settings in the new ViewModel
    QCOMPARE(newSettingsViewModel->audioViewModel()->volume(), 150);
    QCOMPARE(newSettingsViewModel->videoViewModel()->videoProfile(), QString("high-quality"));
    QCOMPARE(newSettingsViewModel->subtitleViewModel()->subtitleFontSize(), 50);

    // Clean up new ViewModel and ConfigManager
    delete newSettingsViewModel;
    delete newConfigManager;
}

QTEST_MAIN(TestSettingsViewModel)
#include "TestSettingsViewModel.moc"
