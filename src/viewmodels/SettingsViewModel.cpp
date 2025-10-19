#include "SettingsViewModel.h"
#include "../models/ConfigManager.h"
#include "AudioViewModel.h"        // Include the new AudioViewModel header
#include "InterfaceOsdViewModel.h" // Include the new InterfaceOsdViewModel header
#include "PerformanceCachingViewModel.h" // Include the new PerformanceCachingViewModel header
#include "PlaybackBehaviorViewModel.h" // Include the new PlaybackBehaviorViewModel header
#include "SubtitleViewModel.h" // Include the new SubtitleViewModel header
#include "VideoViewModel.h"    // Include the new VideoViewModel header
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

SettingsViewModel::SettingsViewModel(ConfigManager *configManager,
                                     QObject *parent)
    : QObject(parent), m_configManager(configManager),
      m_audioViewModel(new AudioViewModel(this)), // Initialize AudioViewModel
      m_videoViewModel(new VideoViewModel(this)), // Initialize VideoViewModel
      m_subtitleViewModel(
          new SubtitleViewModel(this)), // Initialize SubtitleViewModel
      m_playbackBehaviorViewModel(new PlaybackBehaviorViewModel(
          this)), // Initialize PlaybackBehaviorViewModel
      m_performanceCachingViewModel(new PerformanceCachingViewModel(
          this)), // Initialize PerformanceCachingViewModel
      m_interfaceOsdViewModel(
          new InterfaceOsdViewModel(this)) // Initialize InterfaceOsdViewModel
{}

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

void SettingsViewModel::loadSettings() {
  m_configManager->readConfigFile(); // Populate m_configLines in ConfigManager
  m_settings = m_configManager->getSettingsMap();

  // Audio
  if (m_settings.contains("mute")) {
    m_audioViewModel->setMute(m_settings.value("mute") == "yes");
  }
  if (m_settings.contains("volume")) {
    m_audioViewModel->setVolume(m_settings.value("volume").toInt());
  }
  if (m_settings.contains("audio-device")) {
    m_audioViewModel->setAudioDevice(m_settings.value("audio-device"));
  }
  if (m_settings.contains("audio-channels")) {
    m_audioViewModel->setAudioChannels(m_settings.value("audio-channels"));
  }
  if (m_settings.contains("audio-delay")) {
    m_audioViewModel->setAudioDelay(m_settings.value("audio-delay").toDouble());
  }
  if (m_settings.contains("audio-normalize-downmix")) {
    m_audioViewModel->setAudioNormalizeDownmix(
        m_settings.value("audio-normalize-downmix") == "yes");
  }

  // Video
  if (m_settings.contains("profile")) {
    m_videoViewModel->setVideoProfile(m_settings.value("profile"));
  }
  if (m_settings.contains("scale")) {
    m_videoViewModel->setVideoScale(m_settings.value("scale"));
  }
  if (m_settings.contains("cscale")) {
    m_videoViewModel->setVideoCscale(m_settings.value("cscale"));
  }
  if (m_settings.contains("dscale")) {
    m_videoViewModel->setVideoDscale(m_settings.value("dscale"));
  }
  if (m_settings.contains("interpolation")) {
    m_videoViewModel->setVideoInterpolation(m_settings.value("interpolation") ==
                                            "yes");
  }
  if (m_settings.contains("tscale")) {
    m_videoViewModel->setVideoTscale(m_settings.value("tscale"));
  }
  if (m_settings.contains("video-sync")) {
    m_videoViewModel->setVideoVideoSync(m_settings.value("video-sync"));
  }
  if (m_settings.contains("deband")) {
    m_videoViewModel->setVideoDeband(m_settings.value("deband") == "yes");
  }
  if (m_settings.contains("vo")) {
    m_videoViewModel->setVideoVo(m_settings.value("vo"));
  }

  // Subtitles
  if (m_settings.contains("sub-visibility")) {
    m_subtitleViewModel->setSubtitleVisibility(
        m_settings.value("sub-visibility") == "yes");
  }
  if (m_settings.contains("sub-font-size")) {
    m_subtitleViewModel->setSubtitleFontSize(
        m_settings.value("sub-font-size").toInt());
  }
  if (m_settings.contains("sub-color")) {
    m_subtitleViewModel->setSubtitleColor(m_settings.value("sub-color"));
  }
  if (m_settings.contains("slang")) {
    m_subtitleViewModel->setSubtitleLanguages(m_settings.value("slang"));
  }

  // Subtitles
  if (m_settings.contains("sub-auto")) {
    m_subtitleViewModel->setSubAuto(m_settings.value("sub-auto"));
  }
  if (m_settings.contains("sid")) {
    m_subtitleViewModel->setSid(m_settings.value("sid"));
  }
  if (m_settings.contains("sub-forced-only")) {
    m_subtitleViewModel->setSubForcedOnly(m_settings.value("sub-forced-only") ==
                                          "yes");
  }
  if (m_settings.contains("sub-font")) {
    m_subtitleViewModel->setSubFont(m_settings.value("sub-font"));
  }

  // Playback & Behavior
  if (m_settings.contains("resume-playback")) {
    m_playbackBehaviorViewModel->setResumePlayback(
        m_settings.value("resume-playback") == "yes");
  }
  if (m_settings.contains("save-position-on-quit")) {
    m_playbackBehaviorViewModel->setSavePositionOnQuit(
        m_settings.value("save-position-on-quit") == "yes");
  }
  if (m_settings.contains("loop-file")) {
    m_playbackBehaviorViewModel->setLoopFile(m_settings.value("loop-file"));
  }
  if (m_settings.contains("keep-open")) {
    m_playbackBehaviorViewModel->setKeepOpen(m_settings.value("keep-open"));
  }
  if (m_settings.contains("autofit-larger")) {
    m_playbackBehaviorViewModel->setAutofitLarger(
        m_settings.value("autofit-larger"));
  }
  if (m_settings.contains("ytdl-raw-options")) {
    m_playbackBehaviorViewModel->setYtdlRawOptions(
        m_settings.value("ytdl-raw-options"));
  }

  // Performance / Caching
  if (m_settings.contains("cache")) {
    m_performanceCachingViewModel->setCache(m_settings.value("cache") == "yes");
  }
  if (m_settings.contains("cache-secs")) {
    m_performanceCachingViewModel->setCacheSecs(
        m_settings.value("cache-secs").toInt());
  }
  if (m_settings.contains("demuxer-max-bytes")) {
    m_performanceCachingViewModel->setDemuxerMaxBytes(
        m_settings.value("demuxer-max-bytes"));
  }
  if (m_settings.contains("hwdec")) {
    m_performanceCachingViewModel->setHwdec(m_settings.value("hwdec"));
  }
  if (m_settings.contains("hwdec-codecs")) {
    m_performanceCachingViewModel->setHwdecCodecs(
        m_settings.value("hwdec-codecs"));
  }

  // Interface / OSD
  if (m_settings.contains("osd-level")) {
    m_interfaceOsdViewModel->setOsdLevel(m_settings.value("osd-level").toInt());
  }
  if (m_settings.contains("osd-font-size")) {
    m_interfaceOsdViewModel->setOsdFontSize(
        m_settings.value("osd-font-size").toInt());
  }
  if (m_settings.contains("osd-duration")) {
    m_interfaceOsdViewModel->setOsdDuration(
        m_settings.value("osd-duration").toInt());
  }
  if (m_settings.contains("osc")) {
    m_interfaceOsdViewModel->setOsc(m_settings.value("osc") == "yes");
  }
  if (m_settings.contains("screenshot-format")) {
    m_interfaceOsdViewModel->setScreenshotFormat(
        m_settings.value("screenshot-format"));
  }
  if (m_settings.contains("screenshot-directory")) {
    m_interfaceOsdViewModel->setScreenshotDirectory(
        m_settings.value("screenshot-directory"));
  }
  if (m_settings.contains("screenshot-template")) {
    m_interfaceOsdViewModel->setScreenshotTemplate(
        m_settings.value("screenshot-template"));
  }
}

void SettingsViewModel::saveSettings() {
  // Audio
  m_settings["mute"] = m_audioViewModel->mute() ? "yes" : "no";
  m_settings["volume"] = QString::number(m_audioViewModel->volume());
  m_settings["audio-device"] = m_audioViewModel->audioDevice();
  m_settings["audio-channels"] = m_audioViewModel->audioChannels();
  m_settings["audio-delay"] = QString::number(m_audioViewModel->audioDelay());
  m_settings["audio-normalize-downmix"] =
      m_audioViewModel->audioNormalizeDownmix() ? "yes" : "no";

  // Video
  m_settings["profile"] = m_videoViewModel->videoProfile();
  m_settings["scale"] = m_videoViewModel->videoScale();
  m_settings["cscale"] = m_videoViewModel->videoCscale();
  m_settings["dscale"] = m_videoViewModel->videoDscale();
  m_settings["interpolation"] =
      m_videoViewModel->videoInterpolation() ? "yes" : "no";
  m_settings["tscale"] = m_videoViewModel->videoTscale();
  m_settings["video-sync"] = m_videoViewModel->videoVideoSync();
  m_settings["deband"] = m_videoViewModel->videoDeband() ? "yes" : "no";
  m_settings["vo"] = m_videoViewModel->videoVo();

  // Subtitles
  m_settings["sub-visibility"] =
      m_subtitleViewModel->subtitleVisibility() ? "yes" : "no";
  m_settings["sub-font-size"] =
      QString::number(m_subtitleViewModel->subtitleFontSize());
  m_settings["sub-color"] = m_subtitleViewModel->subtitleColor();
  m_settings["slang"] = m_subtitleViewModel->subtitleLanguages();

  // Subtitles
  m_settings["sub-auto"] = m_subtitleViewModel->subAuto();
  m_settings["sid"] = m_subtitleViewModel->sid();
  m_settings["sub-forced-only"] =
      m_subtitleViewModel->subForcedOnly() ? "yes" : "no";
  m_settings["sub-font"] = m_subtitleViewModel->subFont();

  // Playback & Behavior
  m_settings["resume-playback"] =
      m_playbackBehaviorViewModel->resumePlayback() ? "yes" : "no";
  m_settings["save-position-on-quit"] =
      m_playbackBehaviorViewModel->savePositionOnQuit() ? "yes" : "no";
  m_settings["loop-file"] = m_playbackBehaviorViewModel->loopFile();
  m_settings["keep-open"] = m_playbackBehaviorViewModel->keepOpen();
  m_settings["autofit-larger"] = m_playbackBehaviorViewModel->autofitLarger();
  m_settings["ytdl-raw-options"] =
      m_playbackBehaviorViewModel->ytdlRawOptions();

  // Performance / Caching
  m_settings["cache"] = m_performanceCachingViewModel->cache() ? "yes" : "no";
  m_settings["cache-secs"] =
      QString::number(m_performanceCachingViewModel->cacheSecs());
  m_settings["demuxer-max-bytes"] =
      m_performanceCachingViewModel->demuxerMaxBytes();
  m_settings["hwdec"] = m_performanceCachingViewModel->hwdec();
  // Interface / OSD
  m_settings["osd-level"] =
      QString::number(m_interfaceOsdViewModel->osdLevel());
  m_settings["osd-font-size"] =
      QString::number(m_interfaceOsdViewModel->osdFontSize());
  m_settings["osd-duration"] =
      QString::number(m_interfaceOsdViewModel->osdDuration());
  m_settings["osc"] = m_interfaceOsdViewModel->osc() ? "yes" : "no";
  m_settings["screenshot-format"] = m_interfaceOsdViewModel->screenshotFormat();
  m_settings["screenshot-directory"] =
      m_interfaceOsdViewModel->screenshotDirectory();
  m_settings["screenshot-template"] =
      m_interfaceOsdViewModel->screenshotTemplate();

  bool success = m_configManager->saveConfigFile(m_settings);
  emit settingsSaved(success);
}

void SettingsViewModel::loadDefaults() {
  QMap<QString, QString> defaultSettings = parseDefaultSettings();

  m_audioViewModel->setMute(defaultSettings.value("mute", "no") == "yes");
  m_audioViewModel->setVolume(defaultSettings.value("volume", "100").toInt());
  m_audioViewModel->setAudioDevice(
      defaultSettings.value("audio-device", "auto"));
  m_audioViewModel->setAudioChannels(
      defaultSettings.value("audio-channels", "stereo"));
  m_audioViewModel->setAudioDelay(
      defaultSettings.value("audio-delay", "0.0").toDouble());
  m_audioViewModel->setAudioNormalizeDownmix(
      defaultSettings.value("audio-normalize-downmix", "yes") == "yes");

  m_videoViewModel->setVideoProfile(defaultSettings.value("profile", "gpu-hq"));
  m_videoViewModel->setVideoScale(
      defaultSettings.value("scale", "ewa_lanczossharp"));
  m_videoViewModel->setVideoCscale(
      defaultSettings.value("cscale", "ewa_lanczossharp"));
  m_videoViewModel->setVideoDscale(defaultSettings.value("dscale", "mitchell"));
  m_videoViewModel->setVideoInterpolation(
      defaultSettings.value("interpolation", "no") == "yes");
  m_videoViewModel->setVideoTscale(
      defaultSettings.value("tscale", "oversample"));
  m_videoViewModel->setVideoVideoSync(
      defaultSettings.value("video-sync", "display-resample"));
  m_videoViewModel->setVideoDeband(defaultSettings.value("deband", "no") ==
                                   "yes");
  m_videoViewModel->setVideoVo(defaultSettings.value("vo", "gpu"));

  m_subtitleViewModel->setSubtitleVisibility(
      defaultSettings.value("no-sub", "no") == "no");
  m_subtitleViewModel->setSubAuto(defaultSettings.value("sub-auto", "fuzzy"));
  m_subtitleViewModel->setSid(defaultSettings.value("sid", "0"));
  m_subtitleViewModel->setSubtitleLanguages(defaultSettings.value("slang", ""));
  m_subtitleViewModel->setSubForcedOnly(
      defaultSettings.value("sub-forced-only", "no") == "yes");
  m_subtitleViewModel->setSubFont(defaultSettings.value("sub-font", "Sans"));
  m_subtitleViewModel->setSubtitleFontSize(
      defaultSettings.value("sub-font-size", "36").toInt());
  m_subtitleViewModel->setSubtitleColor(
      defaultSettings.value("sub-color", "#FFFFFF"));

  m_playbackBehaviorViewModel->setResumePlayback(
      defaultSettings.value("resume-playback", "no") == "yes");
  m_playbackBehaviorViewModel->setSavePositionOnQuit(
      defaultSettings.value("save-position-on-quit", "no") == "yes");
  m_playbackBehaviorViewModel->setLoopFile(
      defaultSettings.value("loop-file", "no"));
  m_playbackBehaviorViewModel->setKeepOpen(
      defaultSettings.value("keep-open", "no"));
  m_playbackBehaviorViewModel->setAutofitLarger(
      defaultSettings.value("autofit-larger", ""));
  m_playbackBehaviorViewModel->setYtdlRawOptions(
      defaultSettings.value("ytdl-raw-options", ""));

  m_performanceCachingViewModel->setCache(
      defaultSettings.value("cache", "yes") == "yes");
  m_performanceCachingViewModel->setCacheSecs(
      defaultSettings.value("cache-secs", "10").toInt());
  m_performanceCachingViewModel->setDemuxerMaxBytes(
      defaultSettings.value("demuxer-max-bytes", "auto"));
  m_performanceCachingViewModel->setHwdec(
      defaultSettings.value("hwdec", "auto-safe"));
  m_performanceCachingViewModel->setHwdecCodecs(
      defaultSettings.value("hwdec-codecs", "all"));

  m_interfaceOsdViewModel->setOsdLevel(
      defaultSettings.value("osd-level", "1").toInt());
  m_interfaceOsdViewModel->setOsdFontSize(
      defaultSettings.value("osd-font-size", "28").toInt());
  m_interfaceOsdViewModel->setOsdDuration(
      defaultSettings.value("osd-duration", "2000").toInt());
  m_interfaceOsdViewModel->setOsc(defaultSettings.value("osc", "yes") == "yes");
  m_interfaceOsdViewModel->setScreenshotFormat(
      defaultSettings.value("screenshot-format", "png"));
  m_interfaceOsdViewModel->setScreenshotDirectory(
      defaultSettings.value("screenshot-directory", "~"));
  m_interfaceOsdViewModel->setScreenshotTemplate(
      defaultSettings.value("screenshot-template", "%F_%P"));
}

QMap<QString, QString> SettingsViewModel::parseDefaultSettings() {
  QMap<QString, QString> defaultSettings;
  QFile file(":/defaults.json");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Could not open defaults.json from resources.";
    return defaultSettings;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (doc.isNull()) {
    qWarning() << "Failed to parse defaults.json.";
    return defaultSettings;
  }

  if (doc.isObject() && doc.object().contains("options")) {
    QJsonArray optionsArray = doc.object()["options"].toArray();
    for (const QJsonValue &value : optionsArray) {
      if (value.isObject()) {
        QJsonObject obj = value.toObject();
        if (obj.contains("name") && obj.contains("default")) {
          defaultSettings[obj["name"].toString()] = obj["default"].toString();
        }
      }
    }
  }

  return defaultSettings;
}

QMap<QString, QString> SettingsViewModel::parseOptionDescriptions() {
  QMap<QString, QString> optionDescriptions;
  QFile file(":/defaults.json");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Could not open defaults.json from resources.";
    return optionDescriptions;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if (doc.isNull()) {
    qWarning() << "Failed to parse defaults.json.";
    return optionDescriptions;
  }

  if (doc.isObject() && doc.object().contains("options")) {
    QJsonArray optionsArray = doc.object()["options"].toArray();
    for (const QJsonValue &value : optionsArray) {
      if (value.isObject()) {
        QJsonObject obj = value.toObject();
        if (obj.contains("name") && obj.contains("description")) {
          optionDescriptions[obj["name"].toString()] =
              obj["description"].toString();
        }
      }
    }
  }

  return optionDescriptions;
}

QString SettingsViewModel::getRawConfig() {
    return m_configManager->getRawConfig();
}

void SettingsViewModel::applyRawConfig(const QString &configText) {
    m_configManager->parseRawConfig(configText);
    loadSettings(); // Reload settings from the (now updated) config manager
}

QString SettingsViewModel::getOptionDescription(const QString &optionName) {
    static QMap<QString, QString> descriptions = parseOptionDescriptions();
    return descriptions.value(optionName, "MPV configuration option");
}
