#include "../src/models/ConfigManager.h"
#include "../src/viewmodels/SettingsViewModel.h"
#include "../src/viewmodels/SubtitleViewModel.h"
#include "../src/views/SettingsView.h"
#include <QCoreApplication>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QSignalSpy>
#include <QtTest/QTest>

class TestSettingsView : public QObject
{
  Q_OBJECT

  public:
    TestSettingsView()
        : m_configManager(nullptr), m_settingsViewModel(nullptr),
          m_settingsView(nullptr), m_tempDir(nullptr)
    {
    }
    ~TestSettingsView() {}

  private slots:
    void init();
    void cleanup();
    void testInvalidSubtitleColorInput();

  private:
    ConfigManager*     m_configManager;
    SettingsViewModel* m_settingsViewModel;
    SettingsView*      m_settingsView;
    QTemporaryDir*     m_tempDir;
};

void TestSettingsView::init() // Used by Qt Test framework
{
  m_tempDir = new QTemporaryDir;
  if(!m_tempDir->isValid())
  {
    QFAIL("Failed to create temporary directory.");
  }
  QString configFilePath = QDir(m_tempDir->path()).filePath("mpv.conf");

  m_configManager     = new ConfigManager(configFilePath);
  m_settingsViewModel = new SettingsViewModel(m_configManager, nullptr);
  m_settingsView      = new SettingsView(m_settingsViewModel);
  m_settingsView->setTestMode(true);
}

void TestSettingsView::cleanup() // Used by Qt Test framework
{
  delete m_settingsView;
  delete m_settingsViewModel;
  delete m_configManager;
  delete m_tempDir;
}

void TestSettingsView::testInvalidSubtitleColorInput() // Used by Qt Test
                                                       // framework
{
  // Find the subtitle color QLineEdit
  QLineEdit* colorEdit =
      m_settingsView->findChild<QLineEdit*>("subtitleColorLineEdit");
  QVERIFY(colorEdit); // Ensure the line edit is found

  // Simulate typing an invalid color
  QSignalSpy spy(
      m_settingsViewModel->subtitleViewModel(),
      &SubtitleViewModel::errorOccurred); // Spy on the ViewModel's signal

  colorEdit->setText("#GGGGGG");     // Invalid hex color
  QCoreApplication::processEvents(); // Process events to ensure signal is
                                     // delivered

  QCOMPARE(spy.count(),
           1); // Verify that the warning message was displayed once
  QCOMPARE(
      spy.at(0).at(0).toString(),
      "Invalid color format. Please use a valid hex, named, or rgb() color."); // Verify the error
                                                             // message

  // Simulate typing a valid color
  spy.clear();                   // Clear previous signals
  colorEdit->setText("#123456"); // Valid hex color
  QCoreApplication::processEvents();

  QCOMPARE(spy.count(), 0); // No new warning should be displayed
}

QTEST_MAIN(TestSettingsView)
#include "TestSettingsView.moc"
