#include "SettingsViewModel.h"
#include "../models/ConfigManager.h"
#include "../models/ConfigResult.h"
#include "AudioViewModel.h"
#include "InterfaceOsdViewModel.h"
#include "PerformanceCachingViewModel.h"
#include "PlaybackBehaviorViewModel.h"
#include "SubtitleViewModel.h"
#include "VideoViewModel.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <functional>

static auto getOptionBindings(SettingsViewModel *viewModel) -> QList<OptionBinding> {
  return {
      // Audio
      {.key = "mute",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->audioViewModel()->setMute(val == "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->audioViewModel()->mute() ? "yes" : "no";
       }},
      {.key = "volume",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->audioViewModel()->setVolume(val.toInt());
       },
       .getter = [viewModel]() -> QString {
         return QString::number(viewModel->audioViewModel()->volume());
       }},
      {.key = "audio-device",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->audioViewModel()->setAudioDevice(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->audioViewModel()->audioDevice(); }},
      {.key = "audio-channels",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->audioViewModel()->setAudioChannels(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->audioViewModel()->audioChannels(); }},
      {.key = "audio-delay",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->audioViewModel()->setAudioDelay(val.toDouble());
       },
       .getter = [viewModel]() -> QString {
         return QString::number(viewModel->audioViewModel()->audioDelay());
       }},
      {.key = "audio-normalize-downmix",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->audioViewModel()->setAudioNormalizeDownmix(val == "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->audioViewModel()->audioNormalizeDownmix() ? "yes"
                                                                     : "no";
       }},

      // Video
      {.key = "profile",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->videoViewModel()->setVideoProfile(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->videoViewModel()->videoProfile(); }},
      {.key = "scale",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->videoViewModel()->setVideoScale(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->videoViewModel()->videoScale(); }},
      {.key = "cscale",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->videoViewModel()->setVideoCscale(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->videoViewModel()->videoCscale(); }},
      {.key = "dscale",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->videoViewModel()->setVideoDscale(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->videoViewModel()->videoDscale(); }},
      {.key = "interpolation",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->videoViewModel()->setVideoInterpolation(val == "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->videoViewModel()->videoInterpolation() ? "yes"
                                                                  : "no";
       }},
      {.key = "tscale",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->videoViewModel()->setVideoTscale(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->videoViewModel()->videoTscale(); }},
      {.key = "video-sync",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->videoViewModel()->setVideoVideoSync(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->videoViewModel()->videoVideoSync(); }},
      {.key = "deband",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->videoViewModel()->setVideoDeband(val == "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->videoViewModel()->videoDeband() ? "yes" : "no";
       }},
      {.key = "vo",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->videoViewModel()->setVideoVo(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->videoViewModel()->videoVo(); }},

      // Subtitles
      {.key = "sub-visibility",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->subtitleViewModel()->setSubtitleVisibility(val == "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->subtitleViewModel()->subtitleVisibility() ? "yes"
                                                                     : "no";
       }},
      {.key = "sub-font-size",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->subtitleViewModel()->setSubtitleFontSize(val.toInt());
       },
       .getter = [viewModel]() -> QString {
         return QString::number(
             viewModel->subtitleViewModel()->subtitleFontSize());
       }},
      {.key = "sub-color",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->subtitleViewModel()->setSubtitleColor(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->subtitleViewModel()->subtitleColor();
       }},
      {.key = "slang",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->subtitleViewModel()->setSubtitleLanguages(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->subtitleViewModel()->subtitleLanguages();
       }},
      {.key = "sub-auto",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->subtitleViewModel()->setSubAuto(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->subtitleViewModel()->subAuto(); }},
      {.key = "sid",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->subtitleViewModel()->setSid(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->subtitleViewModel()->sid(); }},
      {.key = "sub-forced-only",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->subtitleViewModel()->setSubForcedOnly(val == "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->subtitleViewModel()->subForcedOnly() ? "yes" : "no";
       }},
      {.key = "sub-font",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->subtitleViewModel()->setSubFont(val);
       },
       .getter = [viewModel]() -> QString { return viewModel->subtitleViewModel()->subFont(); }},

      // Playback & Behavior
      {.key = "resume-playback",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->playbackBehaviorViewModel()->setResumePlayback(val ==
                                                                   "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->playbackBehaviorViewModel()->resumePlayback() ? "yes"
                                                                         : "no";
       }},
      {.key = "save-position-on-quit",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->playbackBehaviorViewModel()->setSavePositionOnQuit(val ==
                                                                       "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->playbackBehaviorViewModel()->savePositionOnQuit()
                    ? "yes"
                    : "no";
       }},
      {.key = "loop-file",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->playbackBehaviorViewModel()->setLoopFile(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->playbackBehaviorViewModel()->loopFile();
       }},
      {.key = "keep-open",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->playbackBehaviorViewModel()->setKeepOpen(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->playbackBehaviorViewModel()->keepOpen();
       }},
      {.key = "autofit-larger",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->playbackBehaviorViewModel()->setAutofitLarger(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->playbackBehaviorViewModel()->autofitLarger();
       }},
      {.key = "ytdl-raw-options",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->playbackBehaviorViewModel()->setYtdlRawOptions(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->playbackBehaviorViewModel()->ytdlRawOptions();
       }},

      // Performance / Caching
      {.key = "cache",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->performanceCachingViewModel()->setCache(val == "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->performanceCachingViewModel()->cache() ? "yes"
                                                                  : "no";
       }},
      {.key = "cache-secs",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->performanceCachingViewModel()->setCacheSecs(val.toInt());
       },
       .getter = [viewModel]() -> QString {
         return QString::number(
             viewModel->performanceCachingViewModel()->cacheSecs());
       }},
      {.key = "demuxer-max-bytes",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->performanceCachingViewModel()->setDemuxerMaxBytes(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->performanceCachingViewModel()->demuxerMaxBytes();
       }},
      {.key = "hwdec",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->performanceCachingViewModel()->setHwdec(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->performanceCachingViewModel()->hwdec();
       }},
      {.key = "hwdec-codecs",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->performanceCachingViewModel()->setHwdecCodecs(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->performanceCachingViewModel()->hwdecCodecs();
       }},

      // Interface / OSD
      {.key = "osd-level",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->interfaceOsdViewModel()->setOsdLevel(val.toInt());
       },
       .getter = [viewModel]() -> QString {
         return QString::number(viewModel->interfaceOsdViewModel()->osdLevel());
       }},
      {.key = "osd-font-size",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->interfaceOsdViewModel()->setOsdFontSize(val.toInt());
       },
       .getter = [viewModel]() -> QString {
         return QString::number(
             viewModel->interfaceOsdViewModel()->osdFontSize());
       }},
      {.key = "osd-duration",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->interfaceOsdViewModel()->setOsdDuration(val.toInt());
       },
       .getter = [viewModel]() -> QString {
         return QString::number(
             viewModel->interfaceOsdViewModel()->osdDuration());
       }},
      {.key = "osc",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->interfaceOsdViewModel()->setOsc(val == "yes");
       },
       .getter = [viewModel]() -> const char * {
         return viewModel->interfaceOsdViewModel()->osc() ? "yes" : "no";
       }},
      {.key = "screenshot-format",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->interfaceOsdViewModel()->setScreenshotFormat(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->interfaceOsdViewModel()->screenshotFormat();
       }},
      {.key = "screenshot-directory",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->interfaceOsdViewModel()->setScreenshotDirectory(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->interfaceOsdViewModel()->screenshotDirectory();
       }},
      {.key = "screenshot-template",
       .setter = [viewModel](const QString &val) -> void {
         viewModel->interfaceOsdViewModel()->setScreenshotTemplate(val);
       },
       .getter = [viewModel]() -> QString {
         return viewModel->interfaceOsdViewModel()->screenshotTemplate();
       }},
  };
}

SettingsViewModel::SettingsViewModel(ConfigManager *configManager,
                                     QObject *parent)
    : QObject(parent), m_configManager(configManager),
      m_audioViewModel(new AudioViewModel(this)),
      m_videoViewModel(new VideoViewModel(this)),
      m_subtitleViewModel(new SubtitleViewModel(this)),
      m_playbackBehaviorViewModel(new PlaybackBehaviorViewModel(this)),
      m_performanceCachingViewModel(new PerformanceCachingViewModel(this)),
      m_interfaceOsdViewModel(new InterfaceOsdViewModel(this)) {

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

void SettingsViewModel::loadSettings() {
  if (!m_configManager->configFileExists()) {
    qWarning() << "Config file does not exist.";
    // Handle this case, maybe load defaults or show an empty state
    return;
  }

  m_configManager->readConfigFile(); // This populates internal m_configLines
  m_settings =
      m_configManager
          ->getSettingsMap(); // This gets the QMap from the parsed lines

  const auto bindings = getOptionBindings(this);

  for (const auto &binding : bindings) {
    if (m_settings.contains(binding.key)) {
      binding.setter(m_settings.value(binding.key));
    }
  }

  setIsDirty(false);
}

void SettingsViewModel::saveSettings() {
  const auto bindings = getOptionBindings(this);
  for (const auto &binding : bindings) {
    m_settings.insert(binding.key, binding.getter());
  }

  MPVDeck::ConfigResult result = m_configManager->saveConfigFile(m_settings);
  emit settingsSaved(result.success);
  if (result.success) {
    setIsDirty(false);
  }
}

auto SettingsViewModel::audioViewModel() const -> AudioViewModel * {
  return m_audioViewModel;
}

auto SettingsViewModel::videoViewModel() const -> VideoViewModel * {
  return m_videoViewModel;
}

auto SettingsViewModel::subtitleViewModel() const -> SubtitleViewModel * {
  return m_subtitleViewModel;
}

auto SettingsViewModel::playbackBehaviorViewModel() const
    -> PlaybackBehaviorViewModel * {
  return m_playbackBehaviorViewModel;
}

auto SettingsViewModel::performanceCachingViewModel() const
    -> PerformanceCachingViewModel * {
  return m_performanceCachingViewModel;
}

auto SettingsViewModel::interfaceOsdViewModel() const
    -> InterfaceOsdViewModel * {
  return m_interfaceOsdViewModel;
}

auto SettingsViewModel::isDirty() const -> bool { return m_isDirty; }

void SettingsViewModel::setIsDirty(bool dirty) {
  if (m_isDirty != dirty) {
    m_isDirty = dirty;
    emit isDirtyChanged(m_isDirty);
  }
}

auto SettingsViewModel::autoReloadRawConfig() const -> bool {
  return m_autoReloadRawConfig;
}

void SettingsViewModel::setAutoReloadRawConfig(bool autoReloadRawConfig) {
  if (m_autoReloadRawConfig != autoReloadRawConfig) {
    m_autoReloadRawConfig = autoReloadRawConfig;
    emit autoReloadRawConfigChanged(m_autoReloadRawConfig);
  }
}

// Implement other methods from SettingsViewModel.h if they are not implemented
// yet For example: loadDefaults, parseDefaultSettings, etc. For now, I'll leave
// them as they are, assuming they are either implemented elsewhere or not
// needed for this refactoring.

void SettingsViewModel::loadDefaults() {
  QJsonDocument doc = loadDefaultsJson();
  if (doc.isNull() || !doc.isObject()) {
    qWarning() << "Invalid defaults.json document.";
    return;
  }

  QJsonObject obj = doc.object();
  if (!obj.contains("options") || !obj["options"].isArray()) {
    qWarning() << "defaults.json does not contain an 'options' array.";
    return;
  }

  QJsonArray optionsArray = obj["options"].toArray();
  const auto bindings = getOptionBindings(this);

  for (const auto &value : optionsArray) {
    if (value.isObject()) {
      QJsonObject optionObj = value.toObject();
      QString name = optionObj["name"].toString();
      QString defaultValue = optionObj["default"].toString();

      for (const auto &binding : bindings) {
        if (binding.key == name) {
          binding.setter(defaultValue);
          break;
        }
      }
    }
  }
  setIsDirty(false); // Loading defaults should not mark as dirty initially
}

auto SettingsViewModel::parseDefaultSettings() -> QMap<QString, QString> {
  // TODO: Implement parsing default settings
  return {};
}

auto SettingsViewModel::parseOptionDescriptions() -> QMap<QString, QString> {
  // TODO: Implement parsing option descriptions
  return {};
}

auto SettingsViewModel::getRawConfig() -> QString {
  return m_configManager->getRawConfig();
}

void SettingsViewModel::applyRawConfig(const QString &configText) {
  m_configManager->parseRawConfig(configText);
  // Save the parsed raw config, reusing the lines already parsed
  m_configManager->saveConfigFile(m_configManager->getSettingsMap(), true);
  loadSettings(); // Reload settings from the (now updated) config manager
}

auto SettingsViewModel::getOptionDescription(const QString &optionName) -> QString {
  // TODO: Implement getting option description
  return "";
}

auto SettingsViewModel::settings() const -> QMap<QString, QString> {
  return m_settings;
}

auto SettingsViewModel::loadDefaultsJson() -> QJsonDocument {
  QFile file(":/defaults.json"); // Assuming defaults.json is in Qt resources
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Failed to open defaults.json:" << file.errorString();
    return {};
  }
  QByteArray jsonData = file.readAll();
  file.close();
  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (doc.isNull()) {
    qWarning() << "Failed to parse defaults.json";
  }
  return doc;
}