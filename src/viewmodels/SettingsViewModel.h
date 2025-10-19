#ifndef MPVDECK_SETTINGSVIEWMODEL_H
#define MPVDECK_SETTINGSVIEWMODEL_H

#include <QMap>
#include <QObject>
#include <QString>

const int DEFAULT_VOLUME = 50;
const int DEFAULT_SUBTITLE_FONT_SIZE = 55;

class SettingsViewModel : public QObject {
  Q_OBJECT
  // Audio
  Q_PROPERTY(bool mute READ mute WRITE setMute NOTIFY muteChanged)
  Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
  Q_PROPERTY(QString audioDevice READ audioDevice WRITE setAudioDevice NOTIFY audioDeviceChanged)
  Q_PROPERTY(QString audioChannels READ audioChannels WRITE setAudioChannels
                 NOTIFY audioChannelsChanged)
  Q_PROPERTY(double audioDelay READ audioDelay WRITE setAudioDelay NOTIFY
                 audioDelayChanged)
  Q_PROPERTY(bool audioNormalizeDownmix READ audioNormalizeDownmix WRITE setAudioNormalizeDownmix NOTIFY audioNormalizeDownmixChanged)

  // Playback & Behavior
  Q_PROPERTY(bool resumePlayback READ resumePlayback WRITE setResumePlayback NOTIFY resumePlaybackChanged)
  Q_PROPERTY(bool savePositionOnQuit READ savePositionOnQuit WRITE setSavePositionOnQuit NOTIFY savePositionOnQuitChanged)
  Q_PROPERTY(QString loopFile READ loopFile WRITE setLoopFile NOTIFY loopFileChanged)
  Q_PROPERTY(QString keepOpen READ keepOpen WRITE setKeepOpen NOTIFY keepOpenChanged)
  Q_PROPERTY(QString autofitLarger READ autofitLarger WRITE setAutofitLarger NOTIFY autofitLargerChanged)
  Q_PROPERTY(QString ytdlRawOptions READ ytdlRawOptions WRITE setYtdlRawOptions NOTIFY ytdlRawOptionsChanged)

  // Performance / Caching
  Q_PROPERTY(bool cache READ cache WRITE setCache NOTIFY cacheChanged)
  Q_PROPERTY(int cacheSecs READ cacheSecs WRITE setCacheSecs NOTIFY cacheSecsChanged)
  Q_PROPERTY(QString demuxerMaxBytes READ demuxerMaxBytes WRITE setDemuxerMaxBytes NOTIFY demuxerMaxBytesChanged)
  Q_PROPERTY(QString hwdec READ hwdec WRITE setHwdec NOTIFY hwdecChanged)
  Q_PROPERTY(QString hwdecCodecs READ hwdecCodecs WRITE setHwdecCodecs NOTIFY hwdecCodecsChanged)

  // Interface / OSD
  Q_PROPERTY(int osdLevel READ osdLevel WRITE setOsdLevel NOTIFY osdLevelChanged)
  Q_PROPERTY(int osdFontSize READ osdFontSize WRITE setOsdFontSize NOTIFY osdFontSizeChanged)
  Q_PROPERTY(int osdDuration READ osdDuration WRITE setOsdDuration NOTIFY osdDurationChanged)
  Q_PROPERTY(bool osc READ osc WRITE setOsc NOTIFY oscChanged)
  Q_PROPERTY(QString screenshotFormat READ screenshotFormat WRITE setScreenshotFormat NOTIFY screenshotFormatChanged)
  Q_PROPERTY(QString screenshotDirectory READ screenshotDirectory WRITE setScreenshotDirectory NOTIFY screenshotDirectoryChanged)
  Q_PROPERTY(QString screenshotTemplate READ screenshotTemplate WRITE setScreenshotTemplate NOTIFY screenshotTemplateChanged)

  // Video
  Q_PROPERTY(QString videoVo READ videoVo WRITE setVideoVo NOTIFY videoVoChanged)
  Q_PROPERTY(QString videoProfile READ videoProfile WRITE setVideoProfile NOTIFY
                 videoProfileChanged)
  Q_PROPERTY(QString videoScale READ videoScale WRITE setVideoScale NOTIFY
                 videoScaleChanged)
  Q_PROPERTY(QString videoCscale READ videoCscale WRITE setVideoCscale NOTIFY videoCscaleChanged)
  Q_PROPERTY(QString videoDscale READ videoDscale WRITE setVideoDscale NOTIFY videoDscaleChanged)
  Q_PROPERTY(bool videoInterpolation READ videoInterpolation WRITE setVideoInterpolation NOTIFY videoInterpolationChanged)
  Q_PROPERTY(QString videoTscale READ videoTscale WRITE setVideoTscale NOTIFY videoTscaleChanged)
  Q_PROPERTY(QString videoVideoSync READ videoVideoSync WRITE setVideoVideoSync NOTIFY videoVideoSyncChanged)
  Q_PROPERTY(bool videoDeband READ videoDeband WRITE setVideoDeband NOTIFY
                 videoDebandChanged)

  // Subtitles
  Q_PROPERTY(bool subtitleVisibility READ subtitleVisibility WRITE
                 setSubtitleVisibility NOTIFY subtitleVisibilityChanged)
  Q_PROPERTY(int subtitleFontSize READ subtitleFontSize WRITE
                 setSubtitleFontSize NOTIFY subtitleFontSizeChanged)
  Q_PROPERTY(QString subtitleColor READ subtitleColor WRITE setSubtitleColor
                 NOTIFY subtitleColorChanged)
  Q_PROPERTY(QString subtitleLanguages READ subtitleLanguages WRITE
                 setSubtitleLanguages NOTIFY subtitleLanguagesChanged)

  // Subtitles
  Q_PROPERTY(QString subAuto READ subAuto WRITE setSubAuto NOTIFY subAutoChanged)
  Q_PROPERTY(QString sid READ sid WRITE setSid NOTIFY sidChanged)
  Q_PROPERTY(bool subForcedOnly READ subForcedOnly WRITE setSubForcedOnly NOTIFY subForcedOnlyChanged)
  Q_PROPERTY(QString subFont READ subFont WRITE setSubFont NOTIFY subFontChanged)

public:
  explicit SettingsViewModel(QObject *parent = nullptr);

  void loadSettings();
  void saveSettings();

  [[nodiscard]] auto settings() const -> QMap<QString, QString>;

  // Audio
  [[nodiscard]] auto mute() const -> bool;
  void setMute(bool mute);

  [[nodiscard]] auto volume() const -> int;
  void setVolume(int volume);

  [[nodiscard]] auto audioDevice() const -> QString;
  void setAudioDevice(const QString &audioDevice);

  [[nodiscard]] auto audioChannels() const -> QString;
  void setAudioChannels(const QString &audioChannels);

  [[nodiscard]] auto audioDelay() const -> double;
  void setAudioDelay(double audioDelay);

  [[nodiscard]] auto audioNormalizeDownmix() const -> bool;
  void setAudioNormalizeDownmix(bool audioNormalizeDownmix);

  // Playback & Behavior
  [[nodiscard]] auto resumePlayback() const -> bool;
  void setResumePlayback(bool resumePlayback);

  [[nodiscard]] auto savePositionOnQuit() const -> bool;
  void setSavePositionOnQuit(bool savePositionOnQuit);

  [[nodiscard]] auto loopFile() const -> QString;
  void setLoopFile(const QString &loopFile);

  [[nodiscard]] auto keepOpen() const -> QString;
  void setKeepOpen(const QString &keepOpen);

  [[nodiscard]] auto autofitLarger() const -> QString;
  void setAutofitLarger(const QString &autofitLarger);

  [[nodiscard]] auto ytdlRawOptions() const -> QString;
  void setYtdlRawOptions(const QString &ytdlRawOptions);

  [[nodiscard]] auto cache() const -> bool;
  void setCache(bool cache);

  [[nodiscard]] auto cacheSecs() const -> int;
  void setCacheSecs(int cacheSecs);

  [[nodiscard]] auto demuxerMaxBytes() const -> QString;
  void setDemuxerMaxBytes(const QString &demuxerMaxBytes);

  [[nodiscard]] auto hwdec() const -> QString;
  void setHwdec(const QString &hwdec);

  [[nodiscard]] auto hwdecCodecs() const -> QString;
  void setHwdecCodecs(const QString &hwdecCodecs);

  [[nodiscard]] auto osdLevel() const -> int;
  void setOsdLevel(int osdLevel);

  [[nodiscard]] auto osdFontSize() const -> int;
  void setOsdFontSize(int osdFontSize);

  [[nodiscard]] auto osdDuration() const -> int;
  void setOsdDuration(int osdDuration);

  [[nodiscard]] auto osc() const -> bool;
  void setOsc(bool osc);

  [[nodiscard]] auto screenshotFormat() const -> QString;
  void setScreenshotFormat(const QString &screenshotFormat);

  [[nodiscard]] auto screenshotDirectory() const -> QString;
  void setScreenshotDirectory(const QString &screenshotDirectory);

  [[nodiscard]] auto screenshotTemplate() const -> QString;
  void setScreenshotTemplate(const QString &screenshotTemplate);

  // Video
  [[nodiscard]] auto videoVo() const -> QString;
  void setVideoVo(const QString &videoVo);

  [[nodiscard]] auto videoProfile() const -> QString;
  void setVideoProfile(const QString &videoProfile);

  [[nodiscard]] auto videoScale() const -> QString;
  void setVideoScale(const QString &videoScale);

  [[nodiscard]] auto videoCscale() const -> QString;
  void setVideoCscale(const QString &videoCscale);

  [[nodiscard]] auto videoDscale() const -> QString;
  void setVideoDscale(const QString &videoDscale);

  [[nodiscard]] auto videoInterpolation() const -> bool;
  void setVideoInterpolation(bool videoInterpolation);

  [[nodiscard]] auto videoTscale() const -> QString;
  void setVideoTscale(const QString &videoTscale);

  [[nodiscard]] auto videoVideoSync() const -> QString;
  void setVideoVideoSync(const QString &videoVideoSync);

  [[nodiscard]] auto videoDeband() const -> bool;
  void setVideoDeband(bool videoDeband);

  // Subtitles
  [[nodiscard]] auto subtitleVisibility() const -> bool;
  void setSubtitleVisibility(bool subtitleVisibility);

  [[nodiscard]] auto subtitleFontSize() const -> int;
  void setSubtitleFontSize(int subtitleFontSize);

  [[nodiscard]] auto subtitleColor() const -> QString;
  void setSubtitleColor(const QString &subtitleColor);

  [[nodiscard]] auto subtitleLanguages() const -> QString;
  void setSubtitleLanguages(const QString &subtitleLanguages);

  [[nodiscard]] auto subAuto() const -> QString;
  void setSubAuto(const QString &subAuto);

  [[nodiscard]] auto sid() const -> QString;
  void setSid(const QString &sid);

  [[nodiscard]] auto subForcedOnly() const -> bool;
  void setSubForcedOnly(bool subForcedOnly);

  [[nodiscard]] auto subFont() const -> QString;
  void setSubFont(const QString &subFont);

signals:
  // Audio
  void muteChanged(bool mute);
  void volumeChanged(int volume);
  void audioDeviceChanged(const QString &audioDevice);
  void audioChannelsChanged(const QString &audioChannels);
  void audioDelayChanged(double audioDelay);
  void audioNormalizeDownmixChanged(bool audioNormalizeDownmix);

  // Playback & Behavior
  void resumePlaybackChanged(bool resumePlayback);
  void savePositionOnQuitChanged(bool savePositionOnQuit);
  void loopFileChanged(const QString &loopFile);
  void keepOpenChanged(const QString &keepOpen);
  void autofitLargerChanged(const QString &autofitLarger);
  void ytdlRawOptionsChanged(const QString &ytdlRawOptions);

  // Performance / Caching
  void cacheChanged(bool cache);
  void cacheSecsChanged(int cacheSecs);
  void demuxerMaxBytesChanged(const QString &demuxerMaxBytes);
  void hwdecChanged(const QString &hwdec);
  void hwdecCodecsChanged(const QString &hwdecCodecs);

  // Interface / OSD
  void osdLevelChanged(int osdLevel);
  void osdFontSizeChanged(int osdFontSize);
  void osdDurationChanged(int osdDuration);
  void oscChanged(bool osc);
  void screenshotFormatChanged(const QString &screenshotFormat);
  void screenshotDirectoryChanged(const QString &screenshotDirectory);
  void screenshotTemplateChanged(const QString &screenshotTemplate);

  // Video
  void videoVoChanged(const QString &videoVo);
  void videoProfileChanged(const QString &videoProfile);
  void videoScaleChanged(const QString &videoScale);
  void videoCscaleChanged(const QString &videoCscale);
  void videoDscaleChanged(const QString &videoDscale);
  void videoInterpolationChanged(bool videoInterpolation);
  void videoTscaleChanged(const QString &videoTscale);
  void videoVideoSyncChanged(const QString &videoVideoSync);
  void videoDebandChanged(bool videoDeband);

  // Subtitles
  void subtitleVisibilityChanged(bool subtitleVisibility);
  void subtitleFontSizeChanged(int subtitleFontSize);
  void subtitleColorChanged(const QString &subtitleColor);
  void subtitleLanguagesChanged(const QString &subtitleLanguages);

  void settingsSaved(bool success);

  // Subtitles
  void subAutoChanged(const QString &subAuto);
  void sidChanged(const QString &sid);
  void subForcedOnlyChanged(bool subForcedOnly);
  void subFontChanged(const QString &subFont);

private:
  QMap<QString, QString> m_settings;
  // Audio
  bool m_mute = false;
  int m_volume = DEFAULT_VOLUME;
  QString m_audioDevice;
  QString m_audioChannels;
  double m_audioDelay = 0.0;
  bool m_audioNormalizeDownmix = true;

  // Playback & Behavior
  bool m_resumePlayback = false;
  bool m_savePositionOnQuit = false;
  QString m_loopFile;
  QString m_keepOpen;
  QString m_autofitLarger;
  QString m_ytdlRawOptions;

  // Performance / Caching
  bool m_cache = true;
  int m_cacheSecs = 10;
  QString m_demuxerMaxBytes;
  QString m_hwdec;
  QString m_hwdecCodecs;

  // Interface / OSD
  int m_osdLevel = 1;
  int m_osdFontSize = 28;
  int m_osdDuration = 2000;
  bool m_osc = true;
  QString m_screenshotFormat;
  QString m_screenshotDirectory;
  QString m_screenshotTemplate;

  // Video
  QString m_videoVo;
  QString m_videoProfile;
  QString m_videoScale;
  QString m_videoCscale;
  QString m_videoDscale;
  bool m_videoInterpolation = false;
  QString m_videoTscale;
  QString m_videoVideoSync;
  bool m_videoDeband = false;

  // Subtitles
  bool m_subtitleVisibility = true;
  int m_subtitleFontSize = DEFAULT_SUBTITLE_FONT_SIZE;
  QString m_subtitleColor;
  QString m_subtitleLanguages;

  // Subtitles
  QString m_subAuto;
  QString m_sid;
  bool m_subForcedOnly = false;
  QString m_subFont;
};

#endif // MPVDECK_SETTINGSVIEWMODEL_H
