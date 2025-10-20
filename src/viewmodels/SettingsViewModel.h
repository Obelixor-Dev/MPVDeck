#ifndef MPVDECK_SETTINGSVIEWMODEL_H
#define MPVDECK_SETTINGSVIEWMODEL_H

#include "../models/ConfigManager.h"
#include "AudioViewModel.h"
#include "InterfaceOsdViewModel.h"
#include "PerformanceCachingViewModel.h"
#include "PlaybackBehaviorViewModel.h"
#include "SubtitleViewModel.h"
#include "VideoViewModel.h"
#include <QByteArray>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QString>

class SettingsViewModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(AudioViewModel *audioViewModel READ audioViewModel CONSTANT)
  Q_PROPERTY(VideoViewModel *videoViewModel READ videoViewModel CONSTANT)
  Q_PROPERTY(
      SubtitleViewModel *subtitleViewModel READ subtitleViewModel CONSTANT)
  Q_PROPERTY(PlaybackBehaviorViewModel *playbackBehaviorViewModel READ
                 playbackBehaviorViewModel CONSTANT)
  Q_PROPERTY(PerformanceCachingViewModel *performanceCachingViewModel READ
                 performanceCachingViewModel CONSTANT)
  Q_PROPERTY(InterfaceOsdViewModel *interfaceOsdViewModel READ
                 interfaceOsdViewModel CONSTANT)
  Q_PROPERTY(bool autoReloadRawConfig READ autoReloadRawConfig WRITE
                 setAutoReloadRawConfig NOTIFY autoReloadRawConfigChanged)
  Q_PROPERTY(bool isDirty READ isDirty WRITE setIsDirty NOTIFY isDirtyChanged)

public:
  explicit SettingsViewModel(ConfigManager *configManager,
                             QObject *parent = nullptr);

  void loadSettings();
  void saveSettings();
  void loadDefaults();
  QMap<QString, QString> parseDefaultSettings();
  QMap<QString, QString> parseOptionDescriptions();
  QString getRawConfig();
  void applyRawConfig(const QString &configText);
  QString getOptionDescription(const QString &optionName);

  [[nodiscard]] auto settings() const -> QMap<QString, QString>;
  [[nodiscard]] auto audioViewModel() const
      -> AudioViewModel *; // Getter for AudioViewModel
  [[nodiscard]] auto videoViewModel() const
      -> VideoViewModel *; // Getter for VideoViewModel
  [[nodiscard]] auto subtitleViewModel() const
      -> SubtitleViewModel *; // Getter for SubtitleViewModel
  [[nodiscard]] auto playbackBehaviorViewModel() const
      -> PlaybackBehaviorViewModel *; // Getter for PlaybackBehaviorViewModel
  [[nodiscard]] auto performanceCachingViewModel() const
      -> PerformanceCachingViewModel
          *; // Getter for PerformanceCachingViewModel
  [[nodiscard]] auto interfaceOsdViewModel() const
      -> InterfaceOsdViewModel *; // Getter for InterfaceOsdViewModel
  [[nodiscard]] bool autoReloadRawConfig() const;
  void setAutoReloadRawConfig(bool autoReloadRawConfig);
  [[nodiscard]] bool isDirty() const;

public slots:
  void setIsDirty(bool dirty);

signals:
  void settingsSaved(bool success);
  void autoReloadRawConfigChanged(bool autoReloadRawConfig);
  void isDirtyChanged(bool isDirty);

private:
  QMap<QString, QString> m_settings;
  ConfigManager *m_configManager;         // Member for ConfigManager
  AudioViewModel *m_audioViewModel;       // New member for AudioViewModel
  VideoViewModel *m_videoViewModel;       // New member for VideoViewModel
  SubtitleViewModel *m_subtitleViewModel; // New member for SubtitleViewModel
  PlaybackBehaviorViewModel
      *m_playbackBehaviorViewModel; // New member for PlaybackBehaviorViewModel
  PerformanceCachingViewModel
      *m_performanceCachingViewModel; // New member for
                                      // PerformanceCachingViewModel
  InterfaceOsdViewModel
      *m_interfaceOsdViewModel; // New member for InterfaceOsdViewModel
  bool m_autoReloadRawConfig = false;
  bool m_isDirty = false;
};

#endif // MPVDECK_SETTINGSVIEWMODEL_H
