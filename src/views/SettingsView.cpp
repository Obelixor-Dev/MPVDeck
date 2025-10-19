#include "SettingsView.h"
#include "../models/ConfigManager.h"
#include "../viewmodels/SettingsViewModel.h"
#include <QMessageBox>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

const int MAX_VOLUME = 200;
const double AUDIO_DELAY_SINGLE_STEP = 0.1;
const int MAX_SUBTITLE_FONT_SIZE = 200;

SettingsView::SettingsView(SettingsViewModel *viewModel, QWidget *parent)
    : QMainWindow(parent), m_viewModel(viewModel), m_tabWidget(new QTabWidget(this)) {

  // Create widgets for each tab
  auto *audioTab = new QWidget(this);
  auto *videoTab = new QWidget(this);
  auto *subtitlesTab = new QWidget(this);
  auto *playbackTab = new QWidget(this);
  auto *performanceTab = new QWidget(this);
  auto *interfaceTab = new QWidget(this);

  // Add tabs to the tab widget
  m_tabWidget->addTab(audioTab, "Audio");
  m_tabWidget->addTab(videoTab, "Video");
  m_tabWidget->addTab(subtitlesTab, "Subtitles");
  m_tabWidget->addTab(playbackTab, "Playback & Behavior");
  m_tabWidget->addTab(performanceTab, "Performance / Caching");
  m_tabWidget->addTab(interfaceTab, "Interface / OSD");

  // --- Performance / Caching Tab ---
  auto *performanceLayout = new QVBoxLayout(performanceTab);

  // Cache checkbox
  auto *cacheCheckBox = new QCheckBox("Enable Cache", this);
  cacheCheckBox->setChecked(m_viewModel->cache());
  connect(cacheCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setCache);
  connect(m_viewModel, &SettingsViewModel::cacheChanged, cacheCheckBox,
          &QCheckBox::setChecked);
  performanceLayout->addWidget(cacheCheckBox);

  // Cache Seconds SpinBox
  auto *cacheSecsLayout = new QHBoxLayout();
  auto *cacheSecsLabel = new QLabel("Cache Duration (seconds):", this);
  auto *cacheSecsSpinBox = new QSpinBox(this);
  cacheSecsSpinBox->setRange(0, 600); // Example range, 0 to 10 minutes
  cacheSecsSpinBox->setValue(m_viewModel->cacheSecs());
  connect(cacheSecsSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel, &SettingsViewModel::setCacheSecs);
  connect(m_viewModel, &SettingsViewModel::cacheSecsChanged,
          cacheSecsSpinBox, &QSpinBox::setValue);
  cacheSecsLayout->addWidget(cacheSecsLabel);
  cacheSecsLayout->addWidget(cacheSecsSpinBox);
  performanceLayout->addLayout(cacheSecsLayout);

  // Demuxer Max Bytes LineEdit
  auto *demuxerMaxBytesLayout = new QHBoxLayout();
  auto *demuxerMaxBytesLabel = new QLabel("Demuxer Max Bytes:", this);
  auto *demuxerMaxBytesLineEdit = new QLineEdit(m_viewModel->demuxerMaxBytes(), this);
  connect(demuxerMaxBytesLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setDemuxerMaxBytes);
  connect(m_viewModel, &SettingsViewModel::demuxerMaxBytesChanged,
          demuxerMaxBytesLineEdit, &QLineEdit::setText);
  demuxerMaxBytesLayout->addWidget(demuxerMaxBytesLabel);
  demuxerMaxBytesLayout->addWidget(demuxerMaxBytesLineEdit);
  performanceLayout->addLayout(demuxerMaxBytesLayout);

  // Hardware Decoding (hwdec) ComboBox
  auto *hwdecLayout = new QHBoxLayout();
  auto *hwdecLabel = new QLabel("Hardware Decoding:", this);
  auto *hwdecComboBox = new QComboBox(this);
  hwdecComboBox->addItems({"auto", "auto-safe", "vaapi", "nvdec", "vdpau", "dxva2", "videotoolbox", "none"});
  hwdecComboBox->setCurrentText(m_viewModel->hwdec());
  connect(hwdecComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setHwdec);
  connect(m_viewModel, &SettingsViewModel::hwdecChanged, hwdecComboBox,
          &QComboBox::setCurrentText);
  hwdecLayout->addWidget(hwdecLabel);
  hwdecLayout->addWidget(hwdecComboBox);
  performanceLayout->addLayout(hwdecLayout);

  // Hardware Decoding Codecs (hwdec-codecs) LineEdit
  auto *hwdecCodecsLayout = new QHBoxLayout();
  auto *hwdecCodecsLabel = new QLabel("Hardware Decoding Codecs:", this);
  auto *hwdecCodecsLineEdit = new QLineEdit(m_viewModel->hwdecCodecs(), this);
  connect(hwdecCodecsLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setHwdecCodecs);
  connect(m_viewModel, &SettingsViewModel::hwdecCodecsChanged,
          hwdecCodecsLineEdit, &QLineEdit::setText);
  hwdecCodecsLayout->addWidget(hwdecCodecsLabel);
  hwdecCodecsLayout->addWidget(hwdecCodecsLineEdit);
  performanceLayout->addLayout(hwdecCodecsLayout);

  // --- Interface / OSD Tab ---
  auto *interfaceLayout = new QVBoxLayout(interfaceTab);

  // OSD Level SpinBox
  auto *osdLevelLayout = new QHBoxLayout();
  auto *osdLevelLabel = new QLabel("OSD Level:", this);
  auto *osdLevelSpinBox = new QSpinBox(this);
  osdLevelSpinBox->setRange(0, 3);
  osdLevelSpinBox->setValue(m_viewModel->osdLevel());
  connect(osdLevelSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel, &SettingsViewModel::setOsdLevel);
  connect(m_viewModel, &SettingsViewModel::osdLevelChanged,
          osdLevelSpinBox, &QSpinBox::setValue);
  osdLevelLayout->addWidget(osdLevelLabel);
  osdLevelLayout->addWidget(osdLevelSpinBox);
  interfaceLayout->addLayout(osdLevelLayout);

  // OSD Font Size SpinBox
  auto *osdFontSizeLayout = new QHBoxLayout();
  auto *osdFontSizeLabel = new QLabel("OSD Font Size:", this);
  auto *osdFontSizeSpinBox = new QSpinBox(this);
  osdFontSizeSpinBox->setRange(0, 100); // Example range
  osdFontSizeSpinBox->setValue(m_viewModel->osdFontSize());
  connect(osdFontSizeSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel, &SettingsViewModel::setOsdFontSize);
  connect(m_viewModel, &SettingsViewModel::osdFontSizeChanged,
          osdFontSizeSpinBox, &QSpinBox::setValue);
  osdFontSizeLayout->addWidget(osdFontSizeLabel);
  osdFontSizeLayout->addWidget(osdFontSizeSpinBox);
  interfaceLayout->addLayout(osdFontSizeLayout);

  // OSD Duration SpinBox
  auto *osdDurationLayout = new QHBoxLayout();
  auto *osdDurationLabel = new QLabel("OSD Duration (ms):", this);
  auto *osdDurationSpinBox = new QSpinBox(this);
  osdDurationSpinBox->setRange(0, 10000); // Example range
  osdDurationSpinBox->setValue(m_viewModel->osdDuration());
  connect(osdDurationSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel, &SettingsViewModel::setOsdDuration);
  connect(m_viewModel, &SettingsViewModel::osdDurationChanged,
          osdDurationSpinBox, &QSpinBox::setValue);
  osdDurationLayout->addWidget(osdDurationLabel);
  osdDurationLayout->addWidget(osdDurationSpinBox);
  interfaceLayout->addLayout(osdDurationLayout);

  // On-Screen Controller (osc) checkbox
  auto *oscCheckBox = new QCheckBox("On-Screen Controller", this);
  oscCheckBox->setChecked(m_viewModel->osc());
  connect(oscCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setOsc);
  connect(m_viewModel, &SettingsViewModel::oscChanged, oscCheckBox,
          &QCheckBox::setChecked);
  interfaceLayout->addWidget(oscCheckBox);

  // Screenshot Format ComboBox
  auto *screenshotFormatLayout = new QHBoxLayout();
  auto *screenshotFormatLabel = new QLabel("Screenshot Format:", this);
  auto *screenshotFormatComboBox = new QComboBox(this);
  screenshotFormatComboBox->addItems({"png", "jpg", "bmp"});
  screenshotFormatComboBox->setCurrentText(m_viewModel->screenshotFormat());
  connect(screenshotFormatComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setScreenshotFormat);
  connect(m_viewModel, &SettingsViewModel::screenshotFormatChanged,
          screenshotFormatComboBox, &QComboBox::setCurrentText);
  screenshotFormatLayout->addWidget(screenshotFormatLabel);
  screenshotFormatLayout->addWidget(screenshotFormatComboBox);
  interfaceLayout->addLayout(screenshotFormatLayout);

  // Screenshot Directory LineEdit
  auto *screenshotDirectoryLayout = new QHBoxLayout();
  auto *screenshotDirectoryLabel = new QLabel("Screenshot Directory:", this);
  auto *screenshotDirectoryLineEdit = new QLineEdit(m_viewModel->screenshotDirectory(), this);
  connect(screenshotDirectoryLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setScreenshotDirectory);
  connect(m_viewModel, &SettingsViewModel::screenshotDirectoryChanged,
          screenshotDirectoryLineEdit, &QLineEdit::setText);
  screenshotDirectoryLayout->addWidget(screenshotDirectoryLabel);
  screenshotDirectoryLayout->addWidget(screenshotDirectoryLineEdit);
  interfaceLayout->addLayout(screenshotDirectoryLayout);

  // Screenshot Template LineEdit
  auto *screenshotTemplateLayout = new QHBoxLayout();
  auto *screenshotTemplateLabel = new QLabel("Screenshot Template:", this);
  auto *screenshotTemplateLineEdit = new QLineEdit(m_viewModel->screenshotTemplate(), this);
  connect(screenshotTemplateLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setScreenshotTemplate);
  connect(m_viewModel, &SettingsViewModel::screenshotTemplateChanged,
          screenshotTemplateLineEdit, &QLineEdit::setText);
  screenshotTemplateLayout->addWidget(screenshotTemplateLabel);
  screenshotTemplateLayout->addWidget(screenshotTemplateLineEdit);
  interfaceLayout->addLayout(screenshotTemplateLayout);

  // --- Playback & Behavior Tab ---
  auto *playbackLayout = new QVBoxLayout(playbackTab);

  // Resume Playback checkbox
  auto *resumePlaybackCheckBox = new QCheckBox("Resume Playback", this);
  resumePlaybackCheckBox->setChecked(m_viewModel->resumePlayback());
  connect(resumePlaybackCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setResumePlayback);
  connect(m_viewModel, &SettingsViewModel::resumePlaybackChanged,
          resumePlaybackCheckBox, &QCheckBox::setChecked);
  playbackLayout->addWidget(resumePlaybackCheckBox);

  // Save Position On Quit checkbox
  auto *savePositionOnQuitCheckBox = new QCheckBox("Save Position On Quit", this);
  savePositionOnQuitCheckBox->setChecked(m_viewModel->savePositionOnQuit());
  connect(savePositionOnQuitCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setSavePositionOnQuit);
  connect(m_viewModel, &SettingsViewModel::savePositionOnQuitChanged,
          savePositionOnQuitCheckBox, &QCheckBox::setChecked);
  playbackLayout->addWidget(savePositionOnQuitCheckBox);

  // Loop File ComboBox
  auto *loopFileLayout = new QHBoxLayout();
  auto *loopFileLabel = new QLabel("Loop File:", this);
  auto *loopFileComboBox = new QComboBox(this);
  loopFileComboBox->addItems({"no", "yes", "inf"});
  loopFileComboBox->setCurrentText(m_viewModel->loopFile());
  connect(loopFileComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setLoopFile);
  connect(m_viewModel, &SettingsViewModel::loopFileChanged, loopFileComboBox,
          &QComboBox::setCurrentText);
  loopFileLayout->addWidget(loopFileLabel);
  loopFileLayout->addWidget(loopFileComboBox);
  playbackLayout->addLayout(loopFileLayout);

  // Keep Open ComboBox
  auto *keepOpenLayout = new QHBoxLayout();
  auto *keepOpenLabel = new QLabel("Keep Open:", this);
  auto *keepOpenComboBox = new QComboBox(this);
  keepOpenComboBox->addItems({"no", "yes", "always"});
  keepOpenComboBox->setCurrentText(m_viewModel->keepOpen());
  connect(keepOpenComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setKeepOpen);
  connect(m_viewModel, &SettingsViewModel::keepOpenChanged, keepOpenComboBox,
          &QComboBox::setCurrentText);
  keepOpenLayout->addWidget(keepOpenLabel);
  keepOpenLayout->addWidget(keepOpenComboBox);
  playbackLayout->addLayout(keepOpenLayout);

  // Autofit Larger LineEdit
  auto *autofitLargerLayout = new QHBoxLayout();
  auto *autofitLargerLabel = new QLabel("Autofit Larger:", this);
  auto *autofitLargerLineEdit = new QLineEdit(m_viewModel->autofitLarger(), this);
  connect(autofitLargerLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setAutofitLarger);
  connect(m_viewModel, &SettingsViewModel::autofitLargerChanged,
          autofitLargerLineEdit, &QLineEdit::setText);
  autofitLargerLayout->addWidget(autofitLargerLabel);
  autofitLargerLayout->addWidget(autofitLargerLineEdit);
  playbackLayout->addLayout(autofitLargerLayout);

  // YTDL Raw Options LineEdit
  auto *ytdlRawOptionsLayout = new QHBoxLayout();
  auto *ytdlRawOptionsLabel = new QLabel("YTDL Raw Options:", this);
  auto *ytdlRawOptionsLineEdit = new QLineEdit(m_viewModel->ytdlRawOptions(), this);
  connect(ytdlRawOptionsLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setYtdlRawOptions);
  connect(m_viewModel, &SettingsViewModel::ytdlRawOptionsChanged,
          ytdlRawOptionsLineEdit, &QLineEdit::setText);
  ytdlRawOptionsLayout->addWidget(ytdlRawOptionsLabel);
  ytdlRawOptionsLayout->addWidget(ytdlRawOptionsLineEdit);
  playbackLayout->addLayout(ytdlRawOptionsLayout);

  // --- Audio Tab ---
  auto *audioLayout = new QVBoxLayout(audioTab);

  // Mute checkbox
  auto *muteCheckBox = new QCheckBox("Mute", this);
  muteCheckBox->setChecked(m_viewModel->mute());
  connect(muteCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setMute);
  connect(m_viewModel, &SettingsViewModel::muteChanged, muteCheckBox,
          &QCheckBox::setChecked);
  audioLayout->addWidget(muteCheckBox);

  // Volume slider
  auto *volumeLayout = new QHBoxLayout();
  auto *volumeLabel = new QLabel("Volume:", this);
  auto *volumeSlider = new QSlider(Qt::Horizontal, this);
  volumeSlider->setRange(0, MAX_VOLUME);
  volumeSlider->setValue(m_viewModel->volume());
  auto *volumeValueLabel =
      new QLabel(QString::number(m_viewModel->volume()), this);
  connect(volumeSlider, &QSlider::valueChanged, m_viewModel,
          &SettingsViewModel::setVolume);
  connect(m_viewModel, &SettingsViewModel::volumeChanged, volumeSlider,
          &QSlider::setValue);
  connect(volumeSlider, &QSlider::valueChanged, volumeValueLabel,
          static_cast<void (QLabel::*)(int)>(&QLabel::setNum));
  volumeLayout->addWidget(volumeLabel);
  volumeLayout->addWidget(volumeSlider);
  volumeLayout->addWidget(volumeValueLabel);
  audioLayout->addLayout(volumeLayout);

  // Audio Device
  auto *audioDeviceLayout = new QHBoxLayout();
  auto *audioDeviceLabel = new QLabel("Audio Device:", this);
  auto *audioDeviceComboBox = new QComboBox(this);
  audioDeviceComboBox->addItems({"auto", "alsa", "pulse", "jack", "oss"});
  audioDeviceComboBox->setCurrentText(m_viewModel->audioDevice());
  connect(audioDeviceComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setAudioDevice);
  connect(m_viewModel, &SettingsViewModel::audioDeviceChanged, audioDeviceComboBox,
          &QComboBox::setCurrentText);
  audioDeviceLayout->addWidget(audioDeviceLabel);
  audioDeviceLayout->addWidget(audioDeviceComboBox);
  audioLayout->addLayout(audioDeviceLayout);

  // Audio Normalize Downmix
  auto *audioNormalizeDownmixCheckBox = new QCheckBox("Normalize Downmix", this);
  audioNormalizeDownmixCheckBox->setChecked(m_viewModel->audioNormalizeDownmix());
  connect(audioNormalizeDownmixCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setAudioNormalizeDownmix);
  connect(m_viewModel, &SettingsViewModel::audioNormalizeDownmixChanged,
          audioNormalizeDownmixCheckBox, &QCheckBox::setChecked);
  audioLayout->addWidget(audioNormalizeDownmixCheckBox);

  // Audio Channels
  auto *audioChannelsLayout = new QHBoxLayout();
  auto *audioChannelsLabel = new QLabel("Audio Channels:", this);
  auto *audioChannelsComboBox = new QComboBox(this);
  audioChannelsComboBox->addItems({"stereo", "mono", "5.1", "7.1", "downmix"});
  audioChannelsComboBox->setCurrentText(m_viewModel->audioChannels());
  connect(audioChannelsComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setAudioChannels);
  connect(m_viewModel, &SettingsViewModel::audioChannelsChanged,
          audioChannelsComboBox, &QComboBox::setCurrentText);
  audioChannelsLayout->addWidget(audioChannelsLabel);
  audioChannelsLayout->addWidget(audioChannelsComboBox);
  audioLayout->addLayout(audioChannelsLayout);

  // Audio Delay
  auto *audioDelayLayout = new QHBoxLayout();
  auto *audioDelayLabel = new QLabel("Audio Delay:", this);
  auto *audioDelaySpinBox = new QDoubleSpinBox(this);
  audioDelaySpinBox->setRange(-100.0, 100.0);
  audioDelaySpinBox->setSingleStep(AUDIO_DELAY_SINGLE_STEP);
  audioDelaySpinBox->setValue(m_viewModel->audioDelay());
  connect(audioDelaySpinBox,
          static_cast<void (QDoubleSpinBox::*)(double)>(
              &QDoubleSpinBox::valueChanged),
          m_viewModel, &SettingsViewModel::setAudioDelay);
  connect(m_viewModel, &SettingsViewModel::audioDelayChanged, audioDelaySpinBox,
          &QDoubleSpinBox::setValue);
  audioDelayLayout->addWidget(audioDelayLabel);
  audioDelayLayout->addWidget(audioDelaySpinBox);
  audioLayout->addLayout(audioDelayLayout);

  // --- Video Tab ---
  auto *videoLayout = new QVBoxLayout(videoTab);

  // Profile
  auto *profileLayout = new QHBoxLayout();
  auto *profileLabel = new QLabel("Profile:", this);
  auto *profileComboBox = new QComboBox(this);
  profileComboBox->addItems({"gpu-hq", "gpu", "low-latency", "lavfi-complex", "default"});
  profileComboBox->setCurrentText(m_viewModel->videoProfile());
  connect(profileComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setVideoProfile);
  connect(m_viewModel, &SettingsViewModel::videoProfileChanged, profileComboBox,
          &QComboBox::setCurrentText);
  profileLayout->addWidget(profileLabel);
  profileLayout->addWidget(profileComboBox);
  videoLayout->addLayout(profileLayout);

  // Scale
  auto *scaleLayout = new QHBoxLayout();
  auto *scaleLabel = new QLabel("Scale:", this);
  auto *scaleComboBox = new QComboBox(this);
  scaleComboBox->addItems({"bilinear", "bicubic", "lanczos", "ewa_lanczossharp", "spline36"});
  scaleComboBox->setCurrentText(m_viewModel->videoScale());
  connect(scaleComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setVideoScale);
  connect(m_viewModel, &SettingsViewModel::videoScaleChanged, scaleComboBox,
          &QComboBox::setCurrentText);
  scaleLayout->addWidget(scaleLabel);
  scaleLayout->addWidget(scaleComboBox);
  videoLayout->addLayout(scaleLayout);

  // Deband
  auto *debandCheckBox = new QCheckBox("Deband", this);
  debandCheckBox->setChecked(m_viewModel->videoDeband());
  connect(debandCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setVideoDeband);
  connect(m_viewModel, &SettingsViewModel::videoDebandChanged, debandCheckBox,
          &QCheckBox::setChecked);
  videoLayout->addWidget(debandCheckBox);


  // Video Output (vo)
  auto *voLayout = new QHBoxLayout();
  auto *voLabel = new QLabel("Video Output:", this);
  auto *voComboBox = new QComboBox(this);
  voComboBox->addItems({"gpu", "gpu-next", "x11", "xv", "opengl", "sdl"});
  voComboBox->setCurrentText(m_viewModel->videoVo());
  connect(voComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setVideoVo);
  connect(m_viewModel, &SettingsViewModel::videoVoChanged, voComboBox,
          &QComboBox::setCurrentText);
  voLayout->addWidget(voLabel);
  voLayout->addWidget(voComboBox);
  videoLayout->addLayout(voLayout);

  // Chroma Upscaling (cscale)
  auto *cscaleLayout = new QHBoxLayout();
  auto *cscaleLabel = new QLabel("Chroma Upscaling:", this);
  auto *cscaleComboBox = new QComboBox(this);
  cscaleComboBox->addItems({"bilinear", "bicubic", "lanczos", "ewa_lanczossharp", "spline36"}); // Example values
  cscaleComboBox->setCurrentText(m_viewModel->videoCscale());
  connect(cscaleComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setVideoCscale);
  connect(m_viewModel, &SettingsViewModel::videoCscaleChanged, cscaleComboBox,
          &QComboBox::setCurrentText);
  cscaleLayout->addWidget(cscaleLabel);
  cscaleLayout->addWidget(cscaleComboBox);
  videoLayout->addLayout(cscaleLayout);

  // Downscaling (dscale)
  auto *dscaleLayout = new QHBoxLayout();
  auto *dscaleLabel = new QLabel("Downscaling:", this);
  auto *dscaleComboBox = new QComboBox(this);
  dscaleComboBox->addItems({"bilinear", "bicubic", "lanczos", "mitchell"}); // Example values
  dscaleComboBox->setCurrentText(m_viewModel->videoDscale());
  connect(dscaleComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setVideoDscale);
  connect(m_viewModel, &SettingsViewModel::videoDscaleChanged, dscaleComboBox,
          &QComboBox::setCurrentText);
  dscaleLayout->addWidget(dscaleLabel);
  dscaleLayout->addWidget(dscaleComboBox);
  videoLayout->addLayout(dscaleLayout);

  // Interpolation
  auto *interpolationCheckBox = new QCheckBox("Interpolation", this);
  interpolationCheckBox->setChecked(m_viewModel->videoInterpolation());
  connect(interpolationCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setVideoInterpolation);
  connect(m_viewModel, &SettingsViewModel::videoInterpolationChanged,
          interpolationCheckBox, &QCheckBox::setChecked);
  videoLayout->addWidget(interpolationCheckBox);

  // Temporal Scaling (tscale)
  auto *tscaleLayout = new QHBoxLayout();
  auto *tscaleLabel = new QLabel("Temporal Scaling:", this);
  auto *tscaleComboBox = new QComboBox(this);
  tscaleComboBox->addItems({"oversample", "linear", "nearest"});
  tscaleComboBox->setCurrentText(m_viewModel->videoTscale());
  connect(tscaleComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setVideoTscale);
  connect(m_viewModel, &SettingsViewModel::videoTscaleChanged, tscaleComboBox,
          &QComboBox::setCurrentText);
  tscaleLayout->addWidget(tscaleLabel);
  tscaleLayout->addWidget(tscaleComboBox);
  videoLayout->addLayout(tscaleLayout);

  // Video Sync
  auto *videoSyncLayout = new QHBoxLayout();
  auto *videoSyncLabel = new QLabel("Video Sync:", this);
  auto *videoSyncComboBox = new QComboBox(this);
  videoSyncComboBox->addItems({"display-resample", "audio", "video"});
  videoSyncComboBox->setCurrentText(m_viewModel->videoVideoSync());
  connect(videoSyncComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setVideoVideoSync);
  connect(m_viewModel, &SettingsViewModel::videoVideoSyncChanged, videoSyncComboBox,
          &QComboBox::setCurrentText);
  videoSyncLayout->addWidget(videoSyncLabel);
  videoSyncLayout->addWidget(videoSyncComboBox);
  videoLayout->addLayout(videoSyncLayout);

  // --- Subtitles Tab ---
  auto *subtitlesLayout = new QVBoxLayout(subtitlesTab);

  // Visibility
  auto *subVisibilityCheckBox = new QCheckBox("Visible", this);
  subVisibilityCheckBox->setChecked(m_viewModel->subtitleVisibility());
  connect(subVisibilityCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setSubtitleVisibility);
  connect(m_viewModel, &SettingsViewModel::subtitleVisibilityChanged,
          subVisibilityCheckBox, &QCheckBox::setChecked);
  subtitlesLayout->addWidget(subVisibilityCheckBox);

  // Font Size
  auto *subFontSizeLayout = new QHBoxLayout();
  auto *subFontSizeLabel = new QLabel("Font Size:", this);
  auto *subFontSizeSpinBox = new QSpinBox(this);
  subFontSizeSpinBox->setRange(0, MAX_SUBTITLE_FONT_SIZE);
  subFontSizeSpinBox->setValue(m_viewModel->subtitleFontSize());
  connect(subFontSizeSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel, &SettingsViewModel::setSubtitleFontSize);
  connect(m_viewModel, &SettingsViewModel::subtitleFontSizeChanged,
          subFontSizeSpinBox, &QSpinBox::setValue);
  subFontSizeLayout->addWidget(subFontSizeLabel);
  subFontSizeLayout->addWidget(subFontSizeSpinBox);
  subtitlesLayout->addLayout(subFontSizeLayout);

  // Color
  auto *subColorLayout = new QHBoxLayout();
  auto *subColorLabel = new QLabel("Color:", this);
  auto *subColorLineEdit = new QLineEdit(m_viewModel->subtitleColor(), this);
  auto *subColorButton = new QPushButton("Pick Color", this);
  connect(subColorButton, &QPushButton::clicked, [this, subColorLineEdit]() {
    QColor color = QColorDialog::getColor(QColor(m_viewModel->subtitleColor()),
                                          this, "Pick a color");
    if (color.isValid()) {
      m_viewModel->setSubtitleColor(color.name());
    }
  });
  connect(subColorLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setSubtitleColor);
  connect(m_viewModel, &SettingsViewModel::subtitleColorChanged,
          subColorLineEdit, &QLineEdit::setText);
  subColorLayout->addWidget(subColorLabel);
  subColorLayout->addWidget(subColorLineEdit);
  subColorLayout->addWidget(subColorButton);
  subtitlesLayout->addLayout(subColorLayout);

  // Languages
  auto *slangLayout = new QHBoxLayout();
  auto *slangLabel = new QLabel("Subtitle Languages:", this);
  auto *slangLineEdit = new QLineEdit(m_viewModel->subtitleLanguages(), this);
  connect(slangLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setSubtitleLanguages);
  connect(m_viewModel, &SettingsViewModel::subtitleLanguagesChanged,
          slangLineEdit, &QLineEdit::setText);
  slangLayout->addWidget(slangLabel);
  slangLayout->addWidget(slangLineEdit);
  subtitlesLayout->addLayout(slangLayout);

  // Sub Auto ComboBox
  auto *subAutoLayout = new QHBoxLayout();
  auto *subAutoLabel = new QLabel("Auto Load Subtitles:", this);
  auto *subAutoComboBox = new QComboBox(this);
  subAutoComboBox->addItems({"no", "fuzzy", "exact"});
  subAutoComboBox->setCurrentText(m_viewModel->subAuto());
  connect(subAutoComboBox, &QComboBox::currentTextChanged, m_viewModel,
          &SettingsViewModel::setSubAuto);
  connect(m_viewModel, &SettingsViewModel::subAutoChanged, subAutoComboBox,
          &QComboBox::setCurrentText);
  subAutoLayout->addWidget(subAutoLabel);
  subAutoLayout->addWidget(subAutoComboBox);
  subtitlesLayout->addLayout(subAutoLayout);

  // Subtitle ID (sid) LineEdit
  auto *sidLayout = new QHBoxLayout();
  auto *sidLabel = new QLabel("Subtitle ID:", this);
  auto *sidLineEdit = new QLineEdit(m_viewModel->sid(), this);
  connect(sidLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setSid);
  connect(m_viewModel, &SettingsViewModel::sidChanged, sidLineEdit,
          &QLineEdit::setText);
  sidLayout->addWidget(sidLabel);
  sidLayout->addWidget(sidLineEdit);
  subtitlesLayout->addLayout(sidLayout);

  // Sub Forced Only checkbox
  auto *subForcedOnlyCheckBox = new QCheckBox("Show Forced Subtitles Only", this);
  subForcedOnlyCheckBox->setChecked(m_viewModel->subForcedOnly());
  connect(subForcedOnlyCheckBox, &QCheckBox::toggled, m_viewModel,
          &SettingsViewModel::setSubForcedOnly);
  connect(m_viewModel, &SettingsViewModel::subForcedOnlyChanged,
          subForcedOnlyCheckBox, &QCheckBox::setChecked);
  subtitlesLayout->addWidget(subForcedOnlyCheckBox);

  // Subtitle Font LineEdit
  auto *subFontLayout = new QHBoxLayout();
  auto *subFontLabel = new QLabel("Subtitle Font:", this);
  auto *subFontLineEdit = new QLineEdit(m_viewModel->subFont(), this);
  connect(subFontLineEdit, &QLineEdit::textChanged, m_viewModel,
          &SettingsViewModel::setSubFont);
  connect(m_viewModel, &SettingsViewModel::subFontChanged, subFontLineEdit,
          &QLineEdit::setText);
  subFontLayout->addWidget(subFontLabel);
  subFontLayout->addWidget(subFontLineEdit);
  subtitlesLayout->addLayout(subFontLayout);

  auto *saveButton = new QPushButton("Save", this);

  connect(saveButton, &QPushButton::clicked, this,
          &SettingsView::onSaveButtonClicked);
  connect(m_viewModel, &SettingsViewModel::settingsSaved, this,
          &SettingsView::onSettingsSaved);

  auto *centralWidget = new QWidget(this);
  auto *mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->addWidget(m_tabWidget);
  mainLayout->addWidget(saveButton);

  setCentralWidget(centralWidget);
}

void SettingsView::onSaveButtonClicked() {
  m_viewModel->saveSettings();
}

void SettingsView::onSettingsSaved(bool success) {
  QMessageBox msgBox;
  if (success) {
    msgBox.setText("Settings saved successfully!");
    msgBox.setIcon(QMessageBox::Information);
  } else {
    msgBox.setText("Failed to save settings.");
    msgBox.setIcon(QMessageBox::Critical);
  }
  msgBox.exec();
}

SettingsView::~SettingsView() = default;