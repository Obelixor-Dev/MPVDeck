#include "SettingsViewModel.h"
#include "../models/ConfigManager.h"

SettingsViewModel::SettingsViewModel(QObject *parent) : QObject(parent) {}

void SettingsViewModel::loadSettings() {
  m_settings = ConfigManager::readConfigFile(); // Call static method

  // Audio
  if (m_settings.contains("mute")) {
    setMute(m_settings.value("mute") == "yes");
  }
  if (m_settings.contains("volume")) {
    setVolume(m_settings.value("volume").toInt());
  }
  if (m_settings.contains("audio-device")) {
    setAudioDevice(m_settings.value("audio-device"));
  }
  if (m_settings.contains("audio-channels")) {
    setAudioChannels(m_settings.value("audio-channels"));
  }
  if (m_settings.contains("audio-delay")) {
    setAudioDelay(m_settings.value("audio-delay").toDouble());
  }
  if (m_settings.contains("audio-normalize-downmix")) {
    setAudioNormalizeDownmix(m_settings.value("audio-normalize-downmix") == "yes");
  }

  // Video
  if (m_settings.contains("profile")) {
    setVideoProfile(m_settings.value("profile"));
  }
  if (m_settings.contains("scale")) {
    setVideoScale(m_settings.value("scale"));
  }
  if (m_settings.contains("cscale")) {
    setVideoCscale(m_settings.value("cscale"));
  }
  if (m_settings.contains("dscale")) {
    setVideoDscale(m_settings.value("dscale"));
  }
  if (m_settings.contains("interpolation")) {
    setVideoInterpolation(m_settings.value("interpolation") == "yes");
  }
  if (m_settings.contains("tscale")) {
    setVideoTscale(m_settings.value("tscale"));
  }
  if (m_settings.contains("video-sync")) {
    setVideoVideoSync(m_settings.value("video-sync"));
  }
  if (m_settings.contains("deband")) {
    setVideoDeband(m_settings.value("deband") == "yes");
  }
  if (m_settings.contains("vo")) {
    setVideoVo(m_settings.value("vo"));
  }

  // Subtitles
  if (m_settings.contains("sub-visibility")) {
    setSubtitleVisibility(m_settings.value("sub-visibility") == "yes");
  }
  if (m_settings.contains("sub-font-size")) {
    setSubtitleFontSize(m_settings.value("sub-font-size").toInt());
  }
  if (m_settings.contains("sub-color")) {
    setSubtitleColor(m_settings.value("sub-color"));
  }
  if (m_settings.contains("slang")) {
    setSubtitleLanguages(m_settings.value("slang"));
  }

  // Subtitles
  if (m_settings.contains("sub-auto")) {
    setSubAuto(m_settings.value("sub-auto"));
  }
  if (m_settings.contains("sid")) {
    setSid(m_settings.value("sid"));
  }
  if (m_settings.contains("sub-forced-only")) {
    setSubForcedOnly(m_settings.value("sub-forced-only") == "yes");
  }
  if (m_settings.contains("sub-font")) {
    setSubFont(m_settings.value("sub-font"));
  }

  // Playback & Behavior
  if (m_settings.contains("resume-playback")) {
    setResumePlayback(m_settings.value("resume-playback") == "yes");
  }
  if (m_settings.contains("save-position-on-quit")) {
    setSavePositionOnQuit(m_settings.value("save-position-on-quit") == "yes");
  }
  if (m_settings.contains("loop-file")) {
    setLoopFile(m_settings.value("loop-file"));
  }
  if (m_settings.contains("keep-open")) {
    setKeepOpen(m_settings.value("keep-open"));
  }
  if (m_settings.contains("autofit-larger")) {
    setAutofitLarger(m_settings.value("autofit-larger"));
  }
  if (m_settings.contains("ytdl-raw-options")) {
    setYtdlRawOptions(m_settings.value("ytdl-raw-options"));
  }

  // Performance / Caching
  if (m_settings.contains("cache")) {
    setCache(m_settings.value("cache") == "yes");
  }
  if (m_settings.contains("cache-secs")) {
    setCacheSecs(m_settings.value("cache-secs").toInt());
  }
  if (m_settings.contains("demuxer-max-bytes")) {
    setDemuxerMaxBytes(m_settings.value("demuxer-max-bytes"));
  }
  if (m_settings.contains("hwdec")) {
    setHwdec(m_settings.value("hwdec"));
  }
  if (m_settings.contains("hwdec-codecs")) {
    setHwdecCodecs(m_settings.value("hwdec-codecs"));
  }

  // Interface / OSD
  if (m_settings.contains("osd-level")) {
    setOsdLevel(m_settings.value("osd-level").toInt());
  }
  if (m_settings.contains("osd-font-size")) {
    setOsdFontSize(m_settings.value("osd-font-size").toInt());
  }
  if (m_settings.contains("osd-duration")) {
    setOsdDuration(m_settings.value("osd-duration").toInt());
  }
  if (m_settings.contains("osc")) {
    setOsc(m_settings.value("osc") == "yes");
  }
  if (m_settings.contains("screenshot-format")) {
    setScreenshotFormat(m_settings.value("screenshot-format"));
  }
  if (m_settings.contains("screenshot-directory")) {
    setScreenshotDirectory(m_settings.value("screenshot-directory"));
  }
  if (m_settings.contains("screenshot-template")) {
    setScreenshotTemplate(m_settings.value("screenshot-template"));
  }
}

void SettingsViewModel::saveSettings() {
  // Audio
  m_settings["mute"] = m_mute ? "yes" : "no";
  m_settings["volume"] = QString::number(m_volume);
  m_settings["audio-device"] = m_audioDevice;
  m_settings["audio-channels"] = m_audioChannels;
  m_settings["audio-delay"] = QString::number(m_audioDelay);
  m_settings["audio-normalize-downmix"] = m_audioNormalizeDownmix ? "yes" : "no";

  // Video
  m_settings["profile"] = m_videoProfile;
  m_settings["scale"] = m_videoScale;
  m_settings["cscale"] = m_videoCscale;
  m_settings["dscale"] = m_videoDscale;
  m_settings["interpolation"] = m_videoInterpolation ? "yes" : "no";
  m_settings["tscale"] = m_videoTscale;
  m_settings["video-sync"] = m_videoVideoSync;
  m_settings["deband"] = m_videoDeband ? "yes" : "no";
  m_settings["vo"] = m_videoVo;

  // Subtitles
  m_settings["sub-visibility"] = m_subtitleVisibility ? "yes" : "no";
  m_settings["sub-font-size"] = QString::number(m_subtitleFontSize);
  m_settings["sub-color"] = m_subtitleColor;
  m_settings["slang"] = m_subtitleLanguages;

  // Subtitles
  m_settings["sub-auto"] = m_subAuto;
  m_settings["sid"] = m_sid;
  m_settings["sub-forced-only"] = m_subForcedOnly ? "yes" : "no";
  m_settings["sub-font"] = m_subFont;

  // Playback & Behavior
  m_settings["resume-playback"] = m_resumePlayback ? "yes" : "no";
  m_settings["save-position-on-quit"] = m_savePositionOnQuit ? "yes" : "no";
  m_settings["loop-file"] = m_loopFile;
  m_settings["keep-open"] = m_keepOpen;
  m_settings["autofit-larger"] = m_autofitLarger;
  m_settings["ytdl-raw-options"] = m_ytdlRawOptions;

  bool success = ConfigManager::saveConfigFile(m_settings);
  emit settingsSaved(success);
}

auto SettingsViewModel::settings() const -> QMap<QString, QString> {
  return m_settings;
}

// Audio
auto SettingsViewModel::mute() const -> bool { return m_mute; }

void SettingsViewModel::setMute(bool mute) {
  if (m_mute != mute) {
    m_mute = mute;
    emit muteChanged(m_mute);
  }
}

auto SettingsViewModel::volume() const -> int { return m_volume; }

void SettingsViewModel::setVolume(int volume) {
  if (m_volume != volume) {
    m_volume = volume;
    emit volumeChanged(m_volume);
  }
}

auto SettingsViewModel::audioChannels() const -> QString { return m_audioChannels; }

void SettingsViewModel::setAudioChannels(const QString &audioChannels) {
  if (m_audioChannels != audioChannels) {
    m_audioChannels = audioChannels;
    emit audioChannelsChanged(m_audioChannels);
  }
}

auto SettingsViewModel::audioDelay() const -> double { return m_audioDelay; }

void SettingsViewModel::setAudioDelay(double audioDelay) {
  if (m_audioDelay != audioDelay) {
    m_audioDelay = audioDelay;
    emit audioDelayChanged(m_audioDelay);
  }
}

auto SettingsViewModel::audioDevice() const -> QString { return m_audioDevice; }

void SettingsViewModel::setAudioDevice(const QString &audioDevice) {
  if (m_audioDevice != audioDevice) {
    m_audioDevice = audioDevice;
    emit audioDeviceChanged(m_audioDevice);
  }
}

auto SettingsViewModel::audioNormalizeDownmix() const -> bool { return m_audioNormalizeDownmix; }

void SettingsViewModel::setAudioNormalizeDownmix(bool audioNormalizeDownmix) {
  if (m_audioNormalizeDownmix != audioNormalizeDownmix) {
    m_audioNormalizeDownmix = audioNormalizeDownmix;
    emit audioNormalizeDownmixChanged(audioNormalizeDownmix);
  }
}

// Playback & Behavior
auto SettingsViewModel::resumePlayback() const -> bool { return m_resumePlayback; }

void SettingsViewModel::setResumePlayback(bool resumePlayback) {
  if (m_resumePlayback != resumePlayback) {
    m_resumePlayback = resumePlayback;
    emit resumePlaybackChanged(resumePlayback);
  }
}

auto SettingsViewModel::savePositionOnQuit() const -> bool { return m_savePositionOnQuit; }

void SettingsViewModel::setSavePositionOnQuit(bool savePositionOnQuit) {
  if (m_savePositionOnQuit != savePositionOnQuit) {
    m_savePositionOnQuit = savePositionOnQuit;
    emit savePositionOnQuitChanged(savePositionOnQuit);
  }
}

auto SettingsViewModel::loopFile() const -> QString { return m_loopFile; }

void SettingsViewModel::setLoopFile(const QString &loopFile) {
  if (m_loopFile != loopFile) {
    m_loopFile = loopFile;
    emit loopFileChanged(loopFile);
  }
}

auto SettingsViewModel::keepOpen() const -> QString { return m_keepOpen; }

void SettingsViewModel::setKeepOpen(const QString &keepOpen) {
  if (m_keepOpen != keepOpen) {
    m_keepOpen = keepOpen;
    emit keepOpenChanged(keepOpen);
  }
}

auto SettingsViewModel::autofitLarger() const -> QString { return m_autofitLarger; }

void SettingsViewModel::setAutofitLarger(const QString &autofitLarger) {
  if (m_autofitLarger != autofitLarger) {
    m_autofitLarger = autofitLarger;
    emit autofitLargerChanged(autofitLarger);
  }
}

auto SettingsViewModel::ytdlRawOptions() const -> QString { return m_ytdlRawOptions; }

void SettingsViewModel::setYtdlRawOptions(const QString &ytdlRawOptions) {
  if (m_ytdlRawOptions != ytdlRawOptions) {
    m_ytdlRawOptions = ytdlRawOptions;
    emit ytdlRawOptionsChanged(ytdlRawOptions);
  }
}

// Performance / Caching
auto SettingsViewModel::cache() const -> bool { return m_cache; }

void SettingsViewModel::setCache(bool cache) {
  if (m_cache != cache) {
    m_cache = cache;
    emit cacheChanged(cache);
  }
}

auto SettingsViewModel::cacheSecs() const -> int { return m_cacheSecs; }

void SettingsViewModel::setCacheSecs(int cacheSecs) {
  if (m_cacheSecs != cacheSecs) {
    m_cacheSecs = cacheSecs;
    emit cacheSecsChanged(cacheSecs);
  }
}

auto SettingsViewModel::demuxerMaxBytes() const -> QString { return m_demuxerMaxBytes; }

void SettingsViewModel::setDemuxerMaxBytes(const QString &demuxerMaxBytes) {
  if (m_demuxerMaxBytes != demuxerMaxBytes) {
    m_demuxerMaxBytes = demuxerMaxBytes;
    emit demuxerMaxBytesChanged(demuxerMaxBytes);
  }
}

auto SettingsViewModel::hwdec() const -> QString { return m_hwdec; }

void SettingsViewModel::setHwdec(const QString &hwdec) {
  if (m_hwdec != hwdec) {
    m_hwdec = hwdec;
    emit hwdecChanged(hwdec);
  }
}

auto SettingsViewModel::hwdecCodecs() const -> QString { return m_hwdecCodecs; }

void SettingsViewModel::setHwdecCodecs(const QString &hwdecCodecs) {
  if (m_hwdecCodecs != hwdecCodecs) {
    m_hwdecCodecs = hwdecCodecs;
    emit hwdecCodecsChanged(hwdecCodecs);
  }
}

// Interface / OSD
auto SettingsViewModel::osdLevel() const -> int { return m_osdLevel; }

void SettingsViewModel::setOsdLevel(int osdLevel) {
  if (m_osdLevel != osdLevel) {
    m_osdLevel = osdLevel;
    emit osdLevelChanged(osdLevel);
  }
}

auto SettingsViewModel::osdFontSize() const -> int { return m_osdFontSize; }

void SettingsViewModel::setOsdFontSize(int osdFontSize) {
  if (m_osdFontSize != osdFontSize) {
    m_osdFontSize = osdFontSize;
    emit osdFontSizeChanged(osdFontSize);
  }
}

auto SettingsViewModel::osdDuration() const -> int { return m_osdDuration; }

void SettingsViewModel::setOsdDuration(int osdDuration) {
  if (m_osdDuration != osdDuration) {
    m_osdDuration = osdDuration;
    emit osdDurationChanged(osdDuration);
  }
}

auto SettingsViewModel::osc() const -> bool { return m_osc; }

void SettingsViewModel::setOsc(bool osc) {
  if (m_osc != osc) {
    m_osc = osc;
    emit oscChanged(osc);
  }
}

auto SettingsViewModel::screenshotFormat() const -> QString { return m_screenshotFormat; }

void SettingsViewModel::setScreenshotFormat(const QString &screenshotFormat) {
  if (m_screenshotFormat != screenshotFormat) {
    m_screenshotFormat = screenshotFormat;
    emit screenshotFormatChanged(screenshotFormat);
  }
}

auto SettingsViewModel::screenshotDirectory() const -> QString { return m_screenshotDirectory; }

void SettingsViewModel::setScreenshotDirectory(const QString &screenshotDirectory) {
  if (m_screenshotDirectory != screenshotDirectory) {
    m_screenshotDirectory = screenshotDirectory;
    emit screenshotDirectoryChanged(screenshotDirectory);
  }
}

auto SettingsViewModel::screenshotTemplate() const -> QString { return m_screenshotTemplate; }

void SettingsViewModel::setScreenshotTemplate(const QString &screenshotTemplate) {
  if (m_screenshotTemplate != screenshotTemplate) {
    m_screenshotTemplate = screenshotTemplate;
    emit screenshotTemplateChanged(screenshotTemplate);
  }
}

// Video
auto SettingsViewModel::videoProfile() const -> QString { return m_videoProfile; }

void SettingsViewModel::setVideoProfile(const QString &videoProfile) {
  if (m_videoProfile != videoProfile) {
    m_videoProfile = videoProfile;
    emit videoProfileChanged(m_videoProfile);
  }
}

auto SettingsViewModel::videoScale() const -> QString { return m_videoScale; }

void SettingsViewModel::setVideoScale(const QString &videoScale) {
  if (m_videoScale != videoScale) {
    m_videoScale = videoScale;
    emit videoScaleChanged(m_videoScale);
  }
}

auto SettingsViewModel::videoDeband() const -> bool { return m_videoDeband; }

void SettingsViewModel::setVideoDeband(bool videoDeband) {
  if (m_videoDeband != videoDeband) {
    m_videoDeband = videoDeband;
    emit videoDebandChanged(m_videoDeband);
  }
}

auto SettingsViewModel::videoVo() const -> QString { return m_videoVo; }

void SettingsViewModel::setVideoVo(const QString &videoVo) {
  if (m_videoVo != videoVo) {
    m_videoVo = videoVo;
    emit videoVoChanged(m_videoVo);
  }
}

auto SettingsViewModel::videoCscale() const -> QString { return m_videoCscale; }

void SettingsViewModel::setVideoCscale(const QString &videoCscale) {
  if (m_videoCscale != videoCscale) {
    m_videoCscale = videoCscale;
    emit videoCscaleChanged(m_videoCscale);
  }
}

auto SettingsViewModel::videoDscale() const -> QString { return m_videoDscale; }

void SettingsViewModel::setVideoDscale(const QString &videoDscale) {
  if (m_videoDscale != videoDscale) {
    m_videoDscale = videoDscale;
    emit videoDscaleChanged(m_videoDscale);
  }
}

auto SettingsViewModel::videoInterpolation() const -> bool { return m_videoInterpolation; }

void SettingsViewModel::setVideoInterpolation(bool videoInterpolation) {
  if (m_videoInterpolation != videoInterpolation) {
    m_videoInterpolation = videoInterpolation;
    emit videoInterpolationChanged(m_videoInterpolation);
  }
}

auto SettingsViewModel::videoTscale() const -> QString { return m_videoTscale; }

void SettingsViewModel::setVideoTscale(const QString &videoTscale) {
  if (m_videoTscale != videoTscale) {
    m_videoTscale = videoTscale;
    emit videoTscaleChanged(m_videoTscale);
  }
}

auto SettingsViewModel::videoVideoSync() const -> QString { return m_videoVideoSync; }

void SettingsViewModel::setVideoVideoSync(const QString &videoVideoSync) {
  if (m_videoVideoSync != videoVideoSync) {
    m_videoVideoSync = videoVideoSync;
    emit videoVideoSyncChanged(m_videoVideoSync);
  }
}

// Subtitles
auto SettingsViewModel::subtitleVisibility() const -> bool {
  return m_subtitleVisibility;
}

void SettingsViewModel::setSubtitleVisibility(bool subtitleVisibility) {
  if (m_subtitleVisibility != subtitleVisibility) {
    m_subtitleVisibility = subtitleVisibility;
    emit subtitleVisibilityChanged(m_subtitleVisibility);
  }
}

auto SettingsViewModel::subtitleFontSize() const -> int { return m_subtitleFontSize; }

void SettingsViewModel::setSubtitleFontSize(int subtitleFontSize) {
  if (m_subtitleFontSize != subtitleFontSize) {
    m_subtitleFontSize = subtitleFontSize;
    emit subtitleFontSizeChanged(m_subtitleFontSize);
  }
}

auto SettingsViewModel::subtitleColor() const -> QString { return m_subtitleColor; }

void SettingsViewModel::setSubtitleColor(const QString &subtitleColor) {
  if (m_subtitleColor != subtitleColor) {
    m_subtitleColor = subtitleColor;
    emit subtitleColorChanged(m_subtitleColor);
  }
}

auto SettingsViewModel::subtitleLanguages() const -> QString {
  return m_subtitleLanguages;
}

void SettingsViewModel::setSubtitleLanguages(const QString &subtitleLanguages) {
  if (m_subtitleLanguages != subtitleLanguages) {
    m_subtitleLanguages = subtitleLanguages;
    emit subtitleLanguagesChanged(subtitleLanguages);
  }
}

// Subtitles
auto SettingsViewModel::subAuto() const -> QString { return m_subAuto; }

void SettingsViewModel::setSubAuto(const QString &subAuto) {
  if (m_subAuto != subAuto) {
    m_subAuto = subAuto;
    emit subAutoChanged(subAuto);
  }
}

auto SettingsViewModel::sid() const -> QString { return m_sid; }

void SettingsViewModel::setSid(const QString &sid) {
  if (m_sid != sid) {
    m_sid = sid;
    emit sidChanged(sid);
  }
}

auto SettingsViewModel::subForcedOnly() const -> bool { return m_subForcedOnly; }

void SettingsViewModel::setSubForcedOnly(bool subForcedOnly) {
  if (m_subForcedOnly != subForcedOnly) {
    m_subForcedOnly = subForcedOnly;
    emit subForcedOnlyChanged(subForcedOnly);
  }
}

auto SettingsViewModel::subFont() const -> QString { return m_subFont; }

void SettingsViewModel::setSubFont(const QString &subFont) {
  if (m_subFont != subFont) {
    m_subFont = subFont;
    emit subFontChanged(subFont);
  }
}