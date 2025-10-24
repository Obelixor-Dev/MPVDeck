#include "SettingsViewModel.h"
#include "../models/ConfigManager.h"
#include "../models/ConfigResult.h"
#include "AudioViewModel.h"
#include "DefaultsLoader.h"
#include "InterfaceOsdViewModel.h"
#include "OptionBindings.h"
#include "PerformanceCachingViewModel.h"
#include "PlaybackBehaviorViewModel.h"
#include "SubtitleViewModel.h"
#include "VideoViewModel.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory> // New include
#include <QMessageBox>      // Required for QMessageBox
#include <QTextStream>
#include <functional>

Q_LOGGING_CATEGORY(lcSettingsViewModel,
                   "mpvdeck.settingsviewmodel") // New logging category

QMap<QString, QString> SettingsViewModel::m_optionDescriptions;
bool                   SettingsViewModel::m_optionDescriptionsLoaded = false;

SettingsViewModel::SettingsViewModel(ConfigManager* configManager,
                                     QObject*       parent)
    : QObject(parent), m_configManager(configManager),
      m_audioViewModel(new AudioViewModel(this)),
      m_videoViewModel(new VideoViewModel(this)),
      m_subtitleViewModel(new SubtitleViewModel(this)),
      m_playbackBehaviorViewModel(new PlaybackBehaviorViewModel(this)),
      m_performanceCachingViewModel(new PerformanceCachingViewModel(this)),
      m_interfaceOsdViewModel(new InterfaceOsdViewModel(this)),
      m_defaultsLoader(new DefaultsLoader(this, this))
{

  // Connect all change signals to mark as dirty
  connect(m_audioViewModel, &AudioViewModel::muteChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_audioViewModel, &AudioViewModel::volumeChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_audioViewModel, &AudioViewModel::audioDeviceChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_audioViewModel, &AudioViewModel::audioChannelsChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_audioViewModel, &AudioViewModel::audioDelayChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_audioViewModel, &AudioViewModel::audioNormalizeDownmixChanged, this,
          [this]() -> void { setIsDirty(true); });

  connect(m_videoViewModel, &VideoViewModel::videoProfileChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_videoViewModel, &VideoViewModel::videoScaleChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_videoViewModel, &VideoViewModel::videoCscaleChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_videoViewModel, &VideoViewModel::videoDscaleChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_videoViewModel, &VideoViewModel::videoInterpolationChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_videoViewModel, &VideoViewModel::videoTscaleChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_videoViewModel, &VideoViewModel::videoVideoSyncChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_videoViewModel, &VideoViewModel::videoDebandChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_videoViewModel, &VideoViewModel::videoVoChanged, this,
          [this]() -> void { setIsDirty(true); });

  connect(m_subtitleViewModel, &SubtitleViewModel::subtitleVisibilityChanged,
          this, [this]() -> void { setIsDirty(true); });
  connect(m_subtitleViewModel, &SubtitleViewModel::subtitleFontSizeChanged,
          this, [this]() -> void { setIsDirty(true); });
  connect(m_subtitleViewModel, &SubtitleViewModel::subtitleColorChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_subtitleViewModel, &SubtitleViewModel::subtitleLanguagesChanged,
          this, [this]() -> void { setIsDirty(true); });
  connect(m_subtitleViewModel, &SubtitleViewModel::subAutoChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_subtitleViewModel, &SubtitleViewModel::sidChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_subtitleViewModel, &SubtitleViewModel::subForcedOnlyChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_subtitleViewModel, &SubtitleViewModel::subFontChanged, this,
          [this]() -> void { setIsDirty(true); });

  connect(m_playbackBehaviorViewModel,
          &PlaybackBehaviorViewModel::resumePlaybackChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_playbackBehaviorViewModel,
          &PlaybackBehaviorViewModel::savePositionOnQuitChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_playbackBehaviorViewModel,
          &PlaybackBehaviorViewModel::loopFileChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_playbackBehaviorViewModel,
          &PlaybackBehaviorViewModel::keepOpenChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_playbackBehaviorViewModel,
          &PlaybackBehaviorViewModel::autofitLargerChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_playbackBehaviorViewModel,
          &PlaybackBehaviorViewModel::ytdlRawOptionsChanged, this,
          [this]() -> void { setIsDirty(true); });

  connect(m_performanceCachingViewModel,
          &PerformanceCachingViewModel::cacheChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_performanceCachingViewModel,
          &PerformanceCachingViewModel::cacheSecsChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_performanceCachingViewModel,
          &PerformanceCachingViewModel::demuxerMaxBytesChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_performanceCachingViewModel,
          &PerformanceCachingViewModel::hwdecChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_performanceCachingViewModel,
          &PerformanceCachingViewModel::hwdecCodecsChanged, this,
          [this]() -> void { setIsDirty(true); });

  connect(m_interfaceOsdViewModel, &InterfaceOsdViewModel::osdLevelChanged,
          this, [this]() -> void { setIsDirty(true); });
  connect(m_interfaceOsdViewModel, &InterfaceOsdViewModel::osdFontSizeChanged,
          this, [this]() -> void { setIsDirty(true); });
  connect(m_interfaceOsdViewModel, &InterfaceOsdViewModel::osdDurationChanged,
          this, [this]() -> void { setIsDirty(true); });
  connect(m_interfaceOsdViewModel, &InterfaceOsdViewModel::oscChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_interfaceOsdViewModel,
          &InterfaceOsdViewModel::screenshotFormatChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_interfaceOsdViewModel,
          &InterfaceOsdViewModel::screenshotDirectoryChanged, this,
          [this]() -> void { setIsDirty(true); });
  connect(m_interfaceOsdViewModel,
          &InterfaceOsdViewModel::screenshotTemplateChanged, this,
          [this]() -> void { setIsDirty(true); });
}

void SettingsViewModel::loadSettings()
{
  if(!m_configManager->configFileExists())
  {
    qCWarning(lcSettingsViewModel) << "Config file does not exist.";
    // Handle this case, maybe load defaults or show an empty state
    return;
  }

  m_configManager->readConfigFile(); // This populates internal m_configLines
  m_settings =
      m_configManager
          ->getSettingsMap(); // This gets the QMap from the parsed lines

  const auto bindings = OptionBindings::getAllBindings(this);

  for(const auto& binding : bindings)
  {
    if(m_settings.contains(binding.key))
    {
      binding.setter(m_settings.value(binding.key));
    }
  }

  setIsDirty(false);
}

void SettingsViewModel::saveSettings()
{
  const auto bindings = OptionBindings::getAllBindings(this);
  for(const auto& binding : bindings)
  {
    m_settings.insert(binding.key, binding.getter());
  }

  MPVDeck::ConfigResult result = m_configManager->saveConfigFile(m_settings);
  emit                  settingsSaved(result);
  if(result.success)
  {
    setIsDirty(false);
  }
  else
  {
    QMessageBox::critical(nullptr, "Save Error",
                          "Failed to save configuration: " +
                              result.errorMessage);
  }
}

auto SettingsViewModel::audioViewModel() const -> AudioViewModel*
{
  return m_audioViewModel;
}

auto SettingsViewModel::videoViewModel() const -> VideoViewModel*
{
  return m_videoViewModel;
}

auto SettingsViewModel::subtitleViewModel() const -> SubtitleViewModel*
{
  return m_subtitleViewModel;
}

auto SettingsViewModel::playbackBehaviorViewModel() const
    -> PlaybackBehaviorViewModel*
{
  return m_playbackBehaviorViewModel;
}

auto SettingsViewModel::performanceCachingViewModel() const
    -> PerformanceCachingViewModel*
{
  return m_performanceCachingViewModel;
}

auto SettingsViewModel::interfaceOsdViewModel() const -> InterfaceOsdViewModel*
{
  return m_interfaceOsdViewModel;
}

auto SettingsViewModel::isDirty() const -> bool
{
  return m_isDirty;
}

auto SettingsViewModel::configManager() const -> ConfigManager*
{
  return m_configManager;
}

auto SettingsViewModel::defaultsLoader() const -> DefaultsLoader*
{
  return m_defaultsLoader;
}

void SettingsViewModel::setIsDirty(bool dirty)
{
  if(m_isDirty != dirty)
  {
    m_isDirty = dirty;
    emit isDirtyChanged(m_isDirty);
  }
}

auto SettingsViewModel::autoReloadRawConfig() const -> bool
{ // Used by Qt property system
  return m_autoReloadRawConfig;
}

void SettingsViewModel::setAutoReloadRawConfig(bool autoReloadRawConfig)
{
  Q_UNUSED(autoReloadRawConfig); // Used by Qt property system
  if(m_autoReloadRawConfig != autoReloadRawConfig)
  {
    m_autoReloadRawConfig = autoReloadRawConfig;
    emit autoReloadRawConfigChanged(m_autoReloadRawConfig);
  }
}

auto SettingsViewModel::getOptionDescription(const QString& optionName)
    -> QString
{
  if(!m_optionDescriptionsLoaded)
  {
    QFile file(":/mpv_options_descriptions.json");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      qCWarning(lcSettingsViewModel)
          << "Failed to open mpv_options_descriptions.json:"
          << file.errorString();
      return "";
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if(doc.isNull() || !doc.isObject())
    {
      qCWarning(lcSettingsViewModel)
          << "Failed to parse mpv_options_descriptions.json";
      return "";
    }

    QJsonObject obj = doc.object();
    for(auto it = obj.constBegin(); it != obj.constEnd(); ++it)
    {
      m_optionDescriptions.insert(it.key(), it.value().toString());
    }
    m_optionDescriptionsLoaded = true;
  }

  return m_optionDescriptions.value(optionName, "");
}

auto SettingsViewModel::settings() const -> const QMap<QString, QString>&
{ // Used by Qt property system
  return m_settings;
}

void SettingsViewModel::applyRawConfig(const QString& configText)
{
  m_configManager->parseRawConfig(configText);
  loadSettings();

  MPVDeck::ConfigResult result;
  result.success = true;
  result.errorMessage = "";

  emit settingsSaved(result);
}
