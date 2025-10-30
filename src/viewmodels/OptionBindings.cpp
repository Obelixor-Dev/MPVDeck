#include "OptionBindings.h"
#include "AudioViewModel.h"
#include "InterfaceOsdViewModel.h"
#include "PerformanceCachingViewModel.h"
#include "PlaybackBehaviorViewModel.h"
#include "SettingsViewModel.h"
#include "SubtitleViewModel.h"
#include "VideoViewModel.h"

QList<OptionBinding>
OptionBindings::getAllBindings(QPointer<SettingsViewModel> viewModel)
{
  return {
      // Audio
      {.key    = "mute",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->audioViewModel()->setMute(val == "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->audioViewModel()->mute() ? QStringLiteral("yes")
                                                    : QStringLiteral("no");
       }},
      {.key    = "volume",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->audioViewModel()->setVolume(val.toInt());
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return QString::number(viewModel->audioViewModel()->volume());
       }},
      {.key    = "audio-device",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->audioViewModel()->setAudioDevice(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->audioViewModel()->audioDevice();
       }},
      {.key    = "audio-channels",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->audioViewModel()->setAudioChannels(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->audioViewModel()->audioChannels();
       }},
      {.key    = "audio-delay",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->audioViewModel()->setAudioDelay(val.toDouble());
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return QString::number(viewModel->audioViewModel()->audioDelay());
       }},
      {.key    = "audio-normalize-downmix",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->audioViewModel()->setAudioNormalizeDownmix(val == "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->audioViewModel()->audioNormalizeDownmix()
                    ? QStringLiteral("yes")
                    : QStringLiteral("no");
       }},

      // Video
      {.key    = "profile",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->videoViewModel()->setVideoProfile(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->videoViewModel()->videoProfile();
       }},
      {.key    = "scale",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->videoViewModel()->setVideoScale(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->videoViewModel()->videoScale();
       }},
      {.key    = "cscale",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->videoViewModel()->setVideoCscale(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->videoViewModel()->videoCscale();
       }},
      {.key    = "dscale",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->videoViewModel()->setVideoDscale(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->videoViewModel()->videoDscale();
       }},
      {.key    = "interpolation",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->videoViewModel()->setVideoInterpolation(val == "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->videoViewModel()->videoInterpolation()
                    ? QStringLiteral("yes")
                    : QStringLiteral("no");
       }},
      {.key    = "tscale",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->videoViewModel()->setVideoTscale(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->videoViewModel()->videoTscale();
       }},
      {.key    = "video-sync",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->videoViewModel()->setVideoVideoSync(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->videoViewModel()->videoVideoSync();
       }},
      {.key    = "deband",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->videoViewModel()->setVideoDeband(val == "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->videoViewModel()->videoDeband()
                    ? QStringLiteral("yes")
                    : QStringLiteral("no");
       }},
      {.key    = "vo",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->videoViewModel()->setVideoVo(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->videoViewModel()->videoVo();
       }},

      // Subtitles
      {.key    = "sub-visibility",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->subtitleViewModel()->setSubtitleVisibility(val == "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->subtitleViewModel()->subtitleVisibility()
                    ? QStringLiteral("yes")
                    : QStringLiteral("no");
       }},
      {.key    = "sub-font-size",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->subtitleViewModel()->setSubtitleFontSize(val.toInt());
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return QString::number(
             viewModel->subtitleViewModel()->subtitleFontSize());
       }},
      {.key    = "sub-color",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->subtitleViewModel()->setSubtitleColor(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->subtitleViewModel()->subtitleColor();
       }},
      {.key    = "slang",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->subtitleViewModel()->setSubtitleLanguages(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->subtitleViewModel()->subtitleLanguages();
       }},
      {.key    = "sub-auto",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->subtitleViewModel()->setSubAuto(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->subtitleViewModel()->subAuto();
       }},
      {.key    = "sid",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->subtitleViewModel()->setSid(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->subtitleViewModel()->sid();
       }},
      {.key    = "sub-forced-only",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->subtitleViewModel()->setSubForcedOnly(val == "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->subtitleViewModel()->subForcedOnly()
                    ? QStringLiteral("yes")
                    : QStringLiteral("no");
       }},
      {.key    = "sub-font",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->subtitleViewModel()->setSubFont(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->subtitleViewModel()->subFont();
       }},

      // Playback & Behavior
      {.key    = "resume-playback",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->playbackBehaviorViewModel()->setResumePlayback(val ==
                                                                   "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->playbackBehaviorViewModel()->resumePlayback()
                    ? QStringLiteral("yes")
                    : QStringLiteral("no");
       }},
      {.key    = "save-position-on-quit",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->playbackBehaviorViewModel()->setSavePositionOnQuit(val ==
                                                                       "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->playbackBehaviorViewModel()->savePositionOnQuit()
                    ? QStringLiteral("yes")
                    : QStringLiteral("no");
       }},
      {.key    = "loop-file",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->playbackBehaviorViewModel()->setLoopFile(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->playbackBehaviorViewModel()->loopFile();
       }},
      {.key    = "keep-open",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->playbackBehaviorViewModel()->setKeepOpen(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->playbackBehaviorViewModel()->keepOpen();
       }},
      {.key    = "autofit-larger",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->playbackBehaviorViewModel()->setAutofitLarger(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->playbackBehaviorViewModel()->autofitLarger();
       }},
      {.key    = "ytdl-raw-options",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->playbackBehaviorViewModel()->setYtdlRawOptions(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->playbackBehaviorViewModel()->ytdlRawOptions();
       }},

      // Performance / Caching
      {.key    = "cache",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->performanceCachingViewModel()->setCache(val == "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->performanceCachingViewModel()->cache()
                    ? QStringLiteral("yes")
                    : QStringLiteral("no");
       }},
      {.key    = "cache-secs",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->performanceCachingViewModel()->setCacheSecs(val.toInt());
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return QString::number(
             viewModel->performanceCachingViewModel()->cacheSecs());
       }},
      {.key    = "demuxer-max-bytes",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->performanceCachingViewModel()->setDemuxerMaxBytes(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->performanceCachingViewModel()->demuxerMaxBytes();
       }},
      {.key    = "hwdec",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->performanceCachingViewModel()->setHwdec(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->performanceCachingViewModel()->hwdec();
       }},
      {.key    = "hwdec-codecs",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->performanceCachingViewModel()->setHwdecCodecs(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->performanceCachingViewModel()->hwdecCodecs();
       }},

      // Interface / OSD
      {.key    = "osd-level",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->interfaceOsdViewModel()->setOsdLevel(val.toInt());
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return QString::number(viewModel->interfaceOsdViewModel()->osdLevel());
       }},
      {.key    = "osd-font-size",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->interfaceOsdViewModel()->setOsdFontSize(val.toInt());
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return QString::number(
             viewModel->interfaceOsdViewModel()->osdFontSize());
       }},
      {.key    = "osd-duration",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->interfaceOsdViewModel()->setOsdDuration(val.toInt());
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return QString::number(
             viewModel->interfaceOsdViewModel()->osdDuration());
       }},
      {.key    = "osc",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->interfaceOsdViewModel()->setOsc(val == "yes");
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->interfaceOsdViewModel()->osc()
                    ? QStringLiteral("yes")
                    : QStringLiteral("no");
       }},
      {.key    = "screenshot-format",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->interfaceOsdViewModel()->setScreenshotFormat(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->interfaceOsdViewModel()->screenshotFormat();
       }},
      {.key    = "screenshot-directory",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->interfaceOsdViewModel()->setScreenshotDirectory(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->interfaceOsdViewModel()->screenshotDirectory();
       }},
      {.key    = "screenshot-template",
       .setter = [viewModel](const QString& val) -> void
       {
         if(viewModel.isNull())
         {
           return;
         }
         viewModel->interfaceOsdViewModel()->setScreenshotTemplate(val);
       },
       .getter = [viewModel]() -> QString
       {
         if(viewModel.isNull())
         {
           return {};
         }
         return viewModel->interfaceOsdViewModel()->screenshotTemplate();
       }},
  };
}

QList<OptionBinding>
OptionBindings::getAudioBindings(const SettingsViewModel* viewModel)
{
  return {}; // TODO: Implement specific audio bindings
}

QList<OptionBinding>
OptionBindings::getVideoBindings(const SettingsViewModel* viewModel)
{
  return {}; // TODO: Implement specific video bindings
}