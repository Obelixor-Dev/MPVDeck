#include <QObject>
#include <QtTest/QTest>

#include "../src/viewmodels/OptionBindings.h"
#include "../src/viewmodels/SettingsViewModel.h"
#include "../src/models/ConfigManager.h"

class TestOptionBindings : public QObject
{
  Q_OBJECT

private slots:
  void testGetAllBindings_data();
  void testGetAllBindings();

private:
  ConfigManager*    m_configManager; // Dummy ConfigManager
  SettingsViewModel* m_settingsViewModel; // Dummy SettingsViewModel
};

void TestOptionBindings::testGetAllBindings_data()
{
  QTest::addColumn<QString>("key");
  QTest::addColumn<QString>("initialValue");
  QTest::addColumn<QString>("newValue");

  // Add test data for various option types
  QTest::newRow("audio-mute") << "mute" << "no" << "yes";
  QTest::newRow("volume") << "volume" << "50" << "75";
  QTest::newRow("vo") << "vo" << "gpu" << "opengl";
  QTest::newRow("sub-font-size") << "sub-font-size" << "55" << "40";
  QTest::newRow("save-position-on-quit") << "save-position-on-quit" << "no" << "yes";
  QTest::newRow("cache") << "cache" << "no" << "yes";
  QTest::newRow("osd-level") << "osd-level" << "0" << "1";
}

void TestOptionBindings::testGetAllBindings()
{
  QFETCH(QString, key);
  QFETCH(QString, initialValue);
  QFETCH(QString, newValue);

  // Setup dummy ConfigManager and SettingsViewModel
  m_configManager = new ConfigManager("");
  m_settingsViewModel = new SettingsViewModel(m_configManager, this);

  // Get all bindings
  const auto bindings = OptionBindings::getAllBindings(m_settingsViewModel);

  // Find the binding for the current key
  auto it = std::find_if(bindings.begin(), bindings.end(),
                         [&](const OptionBinding& binding) {
                           return binding.key == key;
                         });

  QVERIFY(it != bindings.end()); // Ensure binding exists

  // Test initial value (setter should set it, getter should retrieve it)
  it->setter(initialValue);
  QCOMPARE(it->getter(), initialValue);

  // Test new value (setter should set it, getter should retrieve it)
  it->setter(newValue);
  QCOMPARE(it->getter(), newValue);

  delete m_settingsViewModel;
  delete m_configManager;
}

QTEST_MAIN(TestOptionBindings)
#include "TestOptionBindings.moc"
