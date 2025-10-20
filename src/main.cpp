#include <QApplication>
#include <QIcon>
#include <QMessageBox>
#include <iostream>
#include "viewmodels/SettingsViewModel.h"
#include "views/SettingsView.h"

const int DEFAULT_WINDOW_WIDTH = 800;
const int DEFAULT_WINDOW_HEIGHT = 600;

// Custom message handler to redirect Qt logs to the console
void messageHandler(QtMsgType type, const QMessageLogContext &context,
                    const QString &msg) {
  QByteArray localMsg = msg.toLocal8Bit();
  const char *file = context.file != nullptr ? context.file : "unknown";
  const char *function = context.function != nullptr ? context.function : "unknown";
  switch (type) {
  case QtDebugMsg:
    qDebug().noquote() << "Debug:" << localMsg.constData() << "(" << file << ":" << context.line << "," << function << ")";
    break;
  case QtInfoMsg:
    qInfo().noquote() << "Info:" << localMsg.constData() << "(" << file << ":" << context.line << "," << function << ")";
    break;
  case QtWarningMsg:
    qWarning().noquote() << "Warning:" << localMsg.constData() << "(" << file << ":" << context.line << "," << function << ")";
    break;
  case QtCriticalMsg:
    qCritical().noquote() << "Critical:" << localMsg.constData() << "(" << file << ":" << context.line << "," << function << ")";
    break;
  case QtFatalMsg:
    qFatal().noquote() << "Fatal:" << localMsg.constData() << "(" << file << ":" << context.line << "," << function << ")";
    abort();
  }
}

auto main(int argc, char *argv[]) -> int {
  qInstallMessageHandler(messageHandler);
  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/mcog.png"));

  // 1. Create Model and ViewModel
  ConfigManager configManager;
  SettingsViewModel viewModel(&configManager);
  viewModel.loadSettings();

  qDebug() << "AudioVM:" << viewModel.audioViewModel();
  qDebug() << "VideoVM:" << viewModel.videoViewModel();
  qDebug() << "PerformanceVM:" << viewModel.performanceCachingViewModel();
  qDebug() << "SubtitleVM:" << viewModel.subtitleViewModel();
  qDebug() << "PlaybackBehaviorVM:" << viewModel.playbackBehaviorViewModel();
  qDebug() << "InterfaceOsdVM:" << viewModel.interfaceOsdViewModel();

  // 2. Create the View
  SettingsView view(&viewModel);
  view.setWindowTitle("MPVDeck");
  view.resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  view.show();

  // Check if config file exists, if not, prompt to create default
  if (!configManager.configFileExists()) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "MPVDeck",
                                  "No mpv.conf found. Would you like to create a default one?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      viewModel.loadDefaults();
      viewModel.saveSettings();
      QMessageBox::information(nullptr, "MPVDeck", "Default mpv.conf created.");
    }
  }

  return QApplication::exec();
}