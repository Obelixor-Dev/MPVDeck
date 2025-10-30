#include "MPVDeckApplication.h"
#include "models/ConfigManager.h"
#include "viewmodels/SettingsViewModel.h"
#include "views/SettingsView.h"
#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QLoggingCategory> // New include
#include <QMessageBox>
#include <QScreen>
#include <QSettings>

Q_LOGGING_CATEGORY(lcMPVDeckApplication,
                   "mpvdeck.application") // New logging category

const int DEFAULT_WINDOW_WIDTH  = 800;
const int DEFAULT_WINDOW_HEIGHT = 600;

// Custom message handler to redirect Qt logs to the console
void messageHandler(QtMsgType type, const QMessageLogContext& context,
                    const QString& msg)
{
  QByteArray  localMsg = msg.toLocal8Bit();
  const char* file     = context.file != nullptr ? context.file : "unknown";
  const char* function =
      context.function != nullptr ? context.function : "unknown";
  switch(type)
  {
  case QtDebugMsg:
    qDebug().noquote() << "Debug:" << localMsg.constData() << "(" << file << ":"
                       << context.line << "," << function << ")";
    break;
  case QtInfoMsg:
    qInfo().noquote() << "Info:" << localMsg.constData() << "(" << file << ":"
                      << context.line << "," << function << ")";
    break;
  case QtWarningMsg:
    qWarning().noquote() << "Warning:" << localMsg.constData() << "(" << file
                         << ":" << context.line << "," << function << ")";
    break;
  case QtCriticalMsg:
    qCritical().noquote() << "Critical:" << localMsg.constData() << "(" << file
                          << ":" << context.line << "," << function << ")";
    break;
  case QtFatalMsg:
    qCritical().noquote() << "Fatal:" << localMsg.constData() << "(" << file << ":"
                       << context.line << "," << function << ")";
  }
}

MPVDeckApplication::MPVDeckApplication(int& argc, char** argv)
    : QApplication(argc, argv)
{
  // Set application and organization name for QSettings
  QCoreApplication::setOrganizationName("MPVDeck");
  QCoreApplication::setApplicationName("MPVDeck");

  // Set default format for QSettings to IniFormat
  QSettings::setDefaultFormat(QSettings::IniFormat);

  setWindowIcon(QIcon(":/mcog.png"));
}

MPVDeckApplication::~MPVDeckApplication() = default;

void MPVDeckApplication::setupMessageHandlers()
{
  qInstallMessageHandler(messageHandler);
  qCDebug(lcMPVDeckApplication) << "Setting up message handlers...";
}

void MPVDeckApplication::setupConfigAndUI()
{
  qCDebug(lcMPVDeckApplication) << "Setting up config and UI...";

  m_configManager = std::make_unique<ConfigManager>();
  m_settingsViewModel =
      std::make_unique<SettingsViewModel>(m_configManager.get());
  m_settingsViewModel->loadSettings();

  qCDebug(lcMPVDeckApplication)
      << "AudioVM:" << m_settingsViewModel->audioViewModel();
  qCDebug(lcMPVDeckApplication)
      << "VideoVM:" << m_settingsViewModel->videoViewModel();
  qCDebug(lcMPVDeckApplication)
      << "PerformanceVM:" << m_settingsViewModel->performanceCachingViewModel();
  qCDebug(lcMPVDeckApplication)
      << "SubtitleVM:" << m_settingsViewModel->subtitleViewModel();
  qCDebug(lcMPVDeckApplication)
      << "PlaybackBehaviorVM:"
      << m_settingsViewModel->playbackBehaviorViewModel();
  qCDebug(lcMPVDeckApplication)
      << "InterfaceOsdVM:" << m_settingsViewModel->interfaceOsdViewModel();

  m_settingsView = std::make_unique<SettingsView>(m_settingsViewModel.get());
  m_settingsView->setWindowTitle("MPVDeck");

  // Check if config file exists, if not, prompt to create default
  if(!m_configManager->configFileExists())
  {
    QMessageBox::StandardButton reply = QMessageBox::question(
        nullptr, "MPVDeck",
        "No mpv.conf found. Would you like to create a default one?",
        QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
      m_settingsViewModel->defaultsLoader()->loadDefaults();
      m_settingsViewModel->saveSettings();
      QMessageBox::information(nullptr, "MPVDeck", "Default mpv.conf created.");
    }
  }
}

void MPVDeckApplication::restoreWindowState()
{
  qCDebug(lcMPVDeckApplication) << "Restoring window state...";
  QSettings settings;
  qCDebug(lcMPVDeckApplication) << "QSettings file:" << settings.fileName();

  bool geometryRestored = false;
  bool stateRestored    = false;

  if(settings.contains("geometry"))
  {
    geometryRestored = m_settingsView->restoreGeometry(
        settings.value("geometry").toByteArray());
  }
  if(settings.contains("windowState"))
  {
    stateRestored = m_settingsView->restoreState(
        settings.value("windowState").toByteArray());
  }

  if(!geometryRestored || !stateRestored)
  {
    qCDebug(lcMPVDeckApplication) << "Window state or geometry not restored. "
                                     "Using default size and centering.";
    m_settingsView->resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    // Center the window
    QScreen* screen = QGuiApplication::primaryScreen();
    if(screen)
    {
      QRect screenGeometry = screen->availableGeometry();
      int   x = (screenGeometry.width() - m_settingsView->width()) / 2;
      int   y = (screenGeometry.height() - m_settingsView->height()) / 2;
      m_settingsView->move(x, y);
    }
  }
  m_settingsView->show();
}

auto MPVDeckApplication::run() -> int
{
  MPVDeckApplication::setupMessageHandlers();
  setupConfigAndUI();
  restoreWindowState();

  return exec();
}