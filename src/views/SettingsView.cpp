#include "SettingsView.h"
#include "../Constants.h"
#include "../models/ConfigManager.h"
#include "../viewmodels/SettingsViewModel.h"
#include <QCheckBox>
#include <QCloseEvent>
#include <QColorDialog>
#include <QComboBox>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QSlider>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QTabWidget>
#include <QTableView>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>
#include <ranges>
#include <numeric>

using namespace MPVDeck::Constants;

SettingsView::SettingsView(SettingsViewModel *viewModel, QWidget *parent)
    : QMainWindow(parent), m_viewModel(viewModel),
      m_tabWidget(new QTabWidget(this)), m_fileMenu(nullptr), m_rawConfigEditor(nullptr),
      m_searchEdit(new QLineEdit(this)) {

  m_tabWidget->addTab(createAudioTab(), "Audio");
  m_tabWidget->addTab(createVideoTab(), "Video");
  m_tabWidget->addTab(createSubtitlesTab(), "Subtitles");
  m_tabWidget->addTab(createPlaybackTab(), "Playback & Behavior");
  m_tabWidget->addTab(createPerformanceTab(), "Performance");
  m_tabWidget->addTab(createInterfaceTab(), "Interface");
  m_tabWidget->addTab(createRawConfigTab(), "Raw Config");

  m_saveButton = new QPushButton("Save", this);

  m_revertButton = new QPushButton("Revert", this);

  auto *loadDefaultsButton = new QPushButton("Load Defaults", this);

  connect(m_saveButton, &QPushButton::clicked, this,

          &SettingsView::onSaveButtonClicked);

  connect(m_revertButton, &QPushButton::clicked, this,

          &SettingsView::onRevertButtonClicked);

  connect(loadDefaultsButton, &QPushButton::clicked, this,

          &SettingsView::onLoadDefaultsButtonClicked);

  connect(m_viewModel, &SettingsViewModel::settingsSaved, this,

          &SettingsView::onSettingsSaved);

  connect(m_viewModel, &SettingsViewModel::isDirtyChanged,

          this, &SettingsView::updateSaveButtonState);

  updateSaveButtonState(m_viewModel->isDirty());

  auto *buttonLayout = new QHBoxLayout();

  buttonLayout->addWidget(m_saveButton);

  buttonLayout->addWidget(m_revertButton);

  buttonLayout->addWidget(loadDefaultsButton);

  auto *centralWidget = new QWidget(this);
  auto *mainLayout = new QVBoxLayout(centralWidget);

  m_searchEdit->setPlaceholderText("🔍 Search settings...");
  connect(m_searchEdit, &QLineEdit::textChanged, this,
          &SettingsView::filterTabs);

  mainLayout->addWidget(m_searchEdit);
  mainLayout->addWidget(m_tabWidget);
  mainLayout->addLayout(buttonLayout);

  setCentralWidget(centralWidget);

  setupMenuBar();
}

void SettingsView::closeEvent(QCloseEvent *event) {
  QSettings settings;
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());
  QMainWindow::closeEvent(event);
}

void SettingsView::onRevertButtonClicked() { m_viewModel->loadSettings(); }

void SettingsView::onLoadDefaultsButtonClicked() {
  m_viewModel->loadDefaults();
}

auto SettingsView::createAudioTab() -> QWidget * {
  auto *audioTab = new QWidget(m_tabWidget);
  auto *audioLayout = new QVBoxLayout(audioTab);

  // Mute checkbox
  auto *muteCheckBox = new QCheckBox("Mute", this);
  muteCheckBox->setChecked(m_viewModel->audioViewModel()->mute());
  muteCheckBox->setToolTip(SettingsViewModel::getOptionDescription("mute"));
  connect(muteCheckBox, &QCheckBox::toggled, m_viewModel->audioViewModel(),
          &AudioViewModel::setMute);
  connect(m_viewModel->audioViewModel(), &AudioViewModel::muteChanged,
          muteCheckBox, &QCheckBox::setChecked);
  audioLayout->addWidget(muteCheckBox);

  // Volume slider
  auto *volumeLayout = new QHBoxLayout();
  auto *volumeLabel = new QLabel("Volume:", this);
  auto *volumeSlider = new QSlider(Qt::Horizontal, this);
  volumeSlider->setRange(0, MPVDeck::Constants::MAX_VOLUME);
  volumeSlider->setValue(m_viewModel->audioViewModel()->volume());
  volumeSlider->setToolTip(SettingsViewModel::getOptionDescription("volume"));
  auto *volumeValueLabel = new QLabel(
      QString::number(m_viewModel->audioViewModel()->volume()), this);
  connect(volumeSlider, &QSlider::valueChanged, m_viewModel->audioViewModel(),
          &AudioViewModel::setVolume);
  connect(m_viewModel->audioViewModel(), &AudioViewModel::volumeChanged,
          volumeSlider, &QSlider::setValue);
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
  audioDeviceComboBox->setCurrentText(
      m_viewModel->audioViewModel()->audioDevice());
  audioDeviceComboBox->setToolTip(
      SettingsViewModel::getOptionDescription("audio-device"));
  connect(audioDeviceComboBox, &QComboBox::currentTextChanged,
          m_viewModel->audioViewModel(), &AudioViewModel::setAudioDevice);
  connect(m_viewModel->audioViewModel(), &AudioViewModel::audioDeviceChanged,
          audioDeviceComboBox, &QComboBox::setCurrentText);
  audioDeviceLayout->addWidget(audioDeviceLabel);
  audioDeviceLayout->addWidget(audioDeviceComboBox);
  audioLayout->addLayout(audioDeviceLayout);

  // Audio Normalize Downmix
  auto *audioNormalizeDownmixCheckBox =
      new QCheckBox("Normalize Downmix", this);
  audioNormalizeDownmixCheckBox->setChecked(
      m_viewModel->audioViewModel()->audioNormalizeDownmix());
  audioNormalizeDownmixCheckBox->setToolTip(
      SettingsViewModel::getOptionDescription("audio-normalize-downmix"));
  connect(audioNormalizeDownmixCheckBox, &QCheckBox::toggled,
          m_viewModel->audioViewModel(),
          &AudioViewModel::setAudioNormalizeDownmix);
  connect(m_viewModel->audioViewModel(),
          &AudioViewModel::audioNormalizeDownmixChanged,
          audioNormalizeDownmixCheckBox, &QCheckBox::setChecked);
  audioLayout->addWidget(audioNormalizeDownmixCheckBox);

  // Audio Channels
  auto *audioChannelsLayout = new QHBoxLayout();
  auto *audioChannelsLabel = new QLabel("Audio Channels:", this);
  auto *audioChannelsComboBox = new QComboBox(this);
  audioChannelsComboBox->addItems({"stereo", "mono", "5.1", "7.1", "downmix"});
  audioChannelsComboBox->setCurrentText(
      m_viewModel->audioViewModel()->audioChannels());
  audioChannelsComboBox->setToolTip(
      SettingsViewModel::getOptionDescription("audio-channels"));
  connect(audioChannelsComboBox, &QComboBox::currentTextChanged,
          m_viewModel->audioViewModel(), &AudioViewModel::setAudioChannels);
  connect(m_viewModel->audioViewModel(), &AudioViewModel::audioChannelsChanged,
          audioChannelsComboBox, &QComboBox::setCurrentText);
  audioChannelsLayout->addWidget(audioChannelsLabel);
  audioChannelsLayout->addWidget(audioChannelsComboBox);
  audioLayout->addLayout(audioChannelsLayout);

  // Audio Delay
  auto *audioDelayLayout = new QHBoxLayout();
  auto *audioDelayLabel = new QLabel("Audio Delay:", this);
  auto *audioDelaySpinBox = new QDoubleSpinBox(this);
  audioDelaySpinBox->setRange(-100.0, 100.0);
  audioDelaySpinBox->setSingleStep(MPVDeck::Constants::AUDIO_DELAY_SINGLE_STEP);
  audioDelaySpinBox->setValue(m_viewModel->audioViewModel()->audioDelay());
  audioDelaySpinBox->setToolTip(
      SettingsViewModel::getOptionDescription("audio-delay"));
  connect(audioDelaySpinBox,
          static_cast<void (QDoubleSpinBox::*)(double)>(
              &QDoubleSpinBox::valueChanged),
          m_viewModel->audioViewModel(), &AudioViewModel::setAudioDelay);
  connect(m_viewModel->audioViewModel(), &AudioViewModel::audioDelayChanged,
          audioDelaySpinBox, &QDoubleSpinBox::setValue);
  audioDelayLayout->addWidget(audioDelayLabel);
  audioDelayLayout->addWidget(audioDelaySpinBox);
  audioLayout->addLayout(audioDelayLayout);

  return audioTab;
}

auto SettingsView::createVideoTab() -> QWidget * {
  auto *videoTab = new QWidget(m_tabWidget);
  auto *videoLayout = new QVBoxLayout(videoTab);

  // Profile
  auto *profileLayout = new QHBoxLayout();
  auto *profileLabel = new QLabel("Profile:", this);
  auto *profileComboBox = new QComboBox(this);
  profileComboBox->addItems(
      {"gpu-hq", "gpu", "low-latency", "lavfi-complex", "default"});
  profileComboBox->setCurrentText(
      m_viewModel->videoViewModel()->videoProfile());
  profileComboBox->setToolTip(SettingsViewModel::getOptionDescription("profile"));
  connect(profileComboBox, &QComboBox::currentTextChanged,
          m_viewModel->videoViewModel(), &VideoViewModel::setVideoProfile);
  connect(m_viewModel->videoViewModel(), &VideoViewModel::videoProfileChanged,
          profileComboBox, &QComboBox::setCurrentText);
  profileLayout->addWidget(profileLabel);
  profileLayout->addWidget(profileComboBox);
  videoLayout->addLayout(profileLayout);

  // Scale
  auto *scaleLayout = new QHBoxLayout();
  auto *scaleLabel = new QLabel("Scale:", this);
  auto *scaleComboBox = new QComboBox(this);
  scaleComboBox->addItems(
      {"bilinear", "bicubic", "lanczos", "ewa_lanczossharp", "spline36"});
  scaleComboBox->setCurrentText(m_viewModel->videoViewModel()->videoScale());
  scaleComboBox->setToolTip(SettingsViewModel::getOptionDescription("scale"));
  connect(scaleComboBox, &QComboBox::currentTextChanged,
          m_viewModel->videoViewModel(), &VideoViewModel::setVideoScale);
  connect(m_viewModel->videoViewModel(), &VideoViewModel::videoScaleChanged,
          scaleComboBox, &QComboBox::setCurrentText);
  scaleLayout->addWidget(scaleLabel);
  scaleLayout->addWidget(scaleComboBox);
  videoLayout->addLayout(scaleLayout);

  // Deband
  auto *debandCheckBox = new QCheckBox("Deband", this);
  debandCheckBox->setChecked(m_viewModel->videoViewModel()->videoDeband());
  debandCheckBox->setToolTip(SettingsViewModel::getOptionDescription("deband"));
  connect(debandCheckBox, &QCheckBox::toggled, m_viewModel->videoViewModel(),
          &VideoViewModel::setVideoDeband);
  connect(m_viewModel->videoViewModel(), &VideoViewModel::videoDebandChanged,
          debandCheckBox, &QCheckBox::setChecked);
  videoLayout->addWidget(debandCheckBox);

  // Video Output (vo)
  auto *voLayout = new QHBoxLayout();
  auto *voLabel = new QLabel("Video Output:", this);
  auto *voComboBox = new QComboBox(this);
  voComboBox->addItems({"gpu", "gpu-next", "x11", "xv", "opengl", "sdl"});
  voComboBox->setCurrentText(m_viewModel->videoViewModel()->videoVo());
  voComboBox->setToolTip(SettingsViewModel::getOptionDescription("vo"));
  connect(voComboBox, &QComboBox::currentTextChanged,
          m_viewModel->videoViewModel(), &VideoViewModel::setVideoVo);
  connect(m_viewModel->videoViewModel(), &VideoViewModel::videoVoChanged,
          voComboBox, &QComboBox::setCurrentText);
  voLayout->addWidget(voLabel);
  voLayout->addWidget(voComboBox);
  videoLayout->addLayout(voLayout);

  // Chroma Upscaling (cscale)
  auto *cscaleLayout = new QHBoxLayout();
  auto *cscaleLabel = new QLabel("Chroma Upscaling:", this);
  auto *cscaleComboBox = new QComboBox(this);
  cscaleComboBox->addItems({"bilinear", "bicubic", "lanczos",
                            "ewa_lanczossharp", "spline36"}); // Example values
  cscaleComboBox->setCurrentText(m_viewModel->videoViewModel()->videoCscale());
  cscaleComboBox->setToolTip(SettingsViewModel::getOptionDescription("cscale"));
  connect(cscaleComboBox, &QComboBox::currentTextChanged,
          m_viewModel->videoViewModel(), &VideoViewModel::setVideoCscale);
  connect(m_viewModel->videoViewModel(), &VideoViewModel::videoCscaleChanged,
          cscaleComboBox, &QComboBox::setCurrentText);
  cscaleLayout->addWidget(cscaleLabel);
  cscaleLayout->addWidget(cscaleComboBox);
  videoLayout->addLayout(cscaleLayout);

  // Downscaling (dscale)
  auto *dscaleLayout = new QHBoxLayout();
  auto *dscaleLabel = new QLabel("Downscaling:", this);
  auto *dscaleComboBox = new QComboBox(this);
  dscaleComboBox->addItems(
      {"bilinear", "bicubic", "lanczos", "mitchell"}); // Example values
  dscaleComboBox->setCurrentText(m_viewModel->videoViewModel()->videoDscale());
  dscaleComboBox->setToolTip(SettingsViewModel::getOptionDescription("dscale"));
  connect(dscaleComboBox, &QComboBox::currentTextChanged,
          m_viewModel->videoViewModel(), &VideoViewModel::setVideoDscale);
  connect(m_viewModel->videoViewModel(), &VideoViewModel::videoDscaleChanged,
          dscaleComboBox, &QComboBox::setCurrentText);
  dscaleLayout->addWidget(dscaleLabel);
  dscaleLayout->addWidget(dscaleComboBox);
  videoLayout->addLayout(dscaleLayout);

  // Interpolation
  auto *interpolationCheckBox = new QCheckBox("Interpolation", this);
  interpolationCheckBox->setChecked(
      m_viewModel->videoViewModel()->videoInterpolation());
  interpolationCheckBox->setToolTip(
      SettingsViewModel::getOptionDescription("interpolation"));
  connect(interpolationCheckBox, &QCheckBox::toggled,
          m_viewModel->videoViewModel(),
          &VideoViewModel::setVideoInterpolation);
  connect(m_viewModel->videoViewModel(),
          &VideoViewModel::videoInterpolationChanged, interpolationCheckBox,
          &QCheckBox::setChecked);
  videoLayout->addWidget(interpolationCheckBox);

  // Temporal Scaling (tscale)
  auto *tscaleLayout = new QHBoxLayout();
  auto *tscaleLabel = new QLabel("Temporal Scaling:", this);
  auto *tscaleComboBox = new QComboBox(this);
  tscaleComboBox->addItems({"oversample", "linear", "nearest"});
  tscaleComboBox->setCurrentText(m_viewModel->videoViewModel()->videoTscale());
  tscaleComboBox->setToolTip(SettingsViewModel::getOptionDescription("tscale"));
  connect(tscaleComboBox, &QComboBox::currentTextChanged,
          m_viewModel->videoViewModel(), &VideoViewModel::setVideoTscale);
  connect(m_viewModel->videoViewModel(), &VideoViewModel::videoTscaleChanged,
          tscaleComboBox, &QComboBox::setCurrentText);
  tscaleLayout->addWidget(tscaleLabel);
  tscaleLayout->addWidget(tscaleComboBox);
  videoLayout->addLayout(tscaleLayout);

  // Video Sync
  auto *videoSyncLayout = new QHBoxLayout();
  auto *videoSyncLabel = new QLabel("Video Sync:", this);
  auto *videoSyncComboBox = new QComboBox(this);
  videoSyncComboBox->addItems({"display-resample", "audio", "video"});
  videoSyncComboBox->setCurrentText(
      m_viewModel->videoViewModel()->videoVideoSync());
  videoSyncComboBox->setToolTip(
      SettingsViewModel::getOptionDescription("video-sync"));
  connect(videoSyncComboBox, &QComboBox::currentTextChanged,
          m_viewModel->videoViewModel(), &VideoViewModel::setVideoVideoSync);
  connect(m_viewModel->videoViewModel(), &VideoViewModel::videoVideoSyncChanged,
          videoSyncComboBox, &QComboBox::setCurrentText);
  videoSyncLayout->addWidget(videoSyncLabel);
  videoSyncLayout->addWidget(videoSyncComboBox);
  videoLayout->addLayout(videoSyncLayout);

  return videoTab;
}

auto SettingsView::createSubtitlesTab() -> QWidget * {
  auto *subtitlesTab = new QWidget(m_tabWidget);
  auto *subtitlesLayout = new QVBoxLayout(subtitlesTab);

  // Visibility
  auto *subVisibilityCheckBox = new QCheckBox("Visible", this);
  subVisibilityCheckBox->setChecked(
      m_viewModel->subtitleViewModel()->subtitleVisibility());
  subVisibilityCheckBox->setToolTip(
      SettingsViewModel::getOptionDescription("sub-visibility"));
  connect(subVisibilityCheckBox, &QCheckBox::toggled,
          m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::setSubtitleVisibility);
  connect(m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::subtitleVisibilityChanged, subVisibilityCheckBox,
          &QCheckBox::setChecked);
  subtitlesLayout->addWidget(subVisibilityCheckBox);

  // Font Size
  auto *subFontSizeLayout = new QHBoxLayout();
  auto *subFontSizeLabel = new QLabel("Font Size:", this);
  auto *subFontSizeSpinBox = new QSpinBox(this);
  subFontSizeSpinBox->setRange(0, MPVDeck::Constants::MAX_SUBTITLE_FONT_SIZE);
  subFontSizeSpinBox->setValue(
      m_viewModel->subtitleViewModel()->subtitleFontSize());
  subFontSizeSpinBox->setToolTip(
      SettingsViewModel::getOptionDescription("sub-font-size"));
  connect(subFontSizeSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::setSubtitleFontSize);
  connect(m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::subtitleFontSizeChanged, subFontSizeSpinBox,
          &QSpinBox::setValue);
  subFontSizeLayout->addWidget(subFontSizeLabel);
  subFontSizeLayout->addWidget(subFontSizeSpinBox);
  subtitlesLayout->addLayout(subFontSizeLayout);

  // Color
  auto *subColorLayout = new QHBoxLayout();
  auto *subColorLabel = new QLabel("Color:", this);
  auto *subColorLineEdit =
      new QLineEdit(m_viewModel->subtitleViewModel()->subtitleColor(), this);
  subColorLineEdit->setToolTip(SettingsViewModel::getOptionDescription("sub-color"));
  auto *subColorButton = new QPushButton("Pick Color", this);
  connect(subColorButton, &QPushButton::clicked, [this, subColorLineEdit]() -> void {
    QColor color = QColorDialog::getColor(
        QColor(m_viewModel->subtitleViewModel()->subtitleColor()), this,
        "Pick a color");
    if (color.isValid()) {
      m_viewModel->subtitleViewModel()->setSubtitleColor(color.name());
    }
  });
  connect(subColorLineEdit, &QLineEdit::textChanged,
          m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::setSubtitleColor);
  connect(m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::subtitleColorChanged, subColorLineEdit,
          &QLineEdit::setText);
  subColorLayout->addWidget(subColorLabel);
  subColorLayout->addWidget(subColorLineEdit);
  subColorLayout->addWidget(subColorButton);
  subtitlesLayout->addLayout(subColorLayout);

  // Languages
  auto *slangLayout = new QHBoxLayout();
  auto *slangLabel = new QLabel("Subtitle Languages:", this);
  auto *slangLineEdit = new QLineEdit(
      m_viewModel->subtitleViewModel()->subtitleLanguages(), this);
  slangLineEdit->setToolTip(SettingsViewModel::getOptionDescription("slang"));
  connect(slangLineEdit, &QLineEdit::textChanged,
          m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::setSubtitleLanguages);
  connect(m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::subtitleLanguagesChanged, slangLineEdit,
          &QLineEdit::setText);
  slangLayout->addWidget(slangLabel);
  slangLayout->addWidget(slangLineEdit);
  subtitlesLayout->addLayout(slangLayout);

  // Sub Auto ComboBox
  auto *subAutoLayout = new QHBoxLayout();
  auto *subAutoLabel = new QLabel("Auto Load Subtitles:", this);
  auto *subAutoComboBox = new QComboBox(this);
  subAutoComboBox->addItems({"no", "fuzzy", "exact"});
  subAutoComboBox->setCurrentText(m_viewModel->subtitleViewModel()->subAuto());
  subAutoComboBox->setToolTip(SettingsViewModel::getOptionDescription("sub-auto"));
  connect(subAutoComboBox, &QComboBox::currentTextChanged,
          m_viewModel->subtitleViewModel(), &SubtitleViewModel::setSubAuto);
  connect(m_viewModel->subtitleViewModel(), &SubtitleViewModel::subAutoChanged,
          subAutoComboBox, &QComboBox::setCurrentText);
  subAutoLayout->addWidget(subAutoLabel);
  subAutoLayout->addWidget(subAutoComboBox);
  subtitlesLayout->addLayout(subAutoLayout);

  // Subtitle ID (sid) LineEdit
  auto *sidLayout = new QHBoxLayout();
  auto *sidLabel = new QLabel("Subtitle ID:", this);
  auto *sidLineEdit =
      new QLineEdit(m_viewModel->subtitleViewModel()->sid(), this);
  sidLineEdit->setToolTip(SettingsViewModel::getOptionDescription("sid"));
  connect(sidLineEdit, &QLineEdit::textChanged,
          m_viewModel->subtitleViewModel(), &SubtitleViewModel::setSid);
  connect(m_viewModel->subtitleViewModel(), &SubtitleViewModel::sidChanged,
          sidLineEdit, &QLineEdit::setText);
  sidLayout->addWidget(sidLabel);
  sidLayout->addWidget(sidLineEdit);
  subtitlesLayout->addLayout(sidLayout);

  // Sub Forced Only checkbox
  auto *subForcedOnlyCheckBox =
      new QCheckBox("Show Forced Subtitles Only", this);
  subForcedOnlyCheckBox->setChecked(
      m_viewModel->subtitleViewModel()->subForcedOnly());
  subForcedOnlyCheckBox->setToolTip(
      SettingsViewModel::getOptionDescription("sub-forced-only"));
  connect(subForcedOnlyCheckBox, &QCheckBox::toggled,
          m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::setSubForcedOnly);
  connect(m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::subForcedOnlyChanged, subForcedOnlyCheckBox,
          &QCheckBox::setChecked);
  subtitlesLayout->addWidget(subForcedOnlyCheckBox);

  // Subtitle Font LineEdit
  auto *subFontLayout = new QHBoxLayout();
  auto *subFontLabel = new QLabel("Subtitle Font:", this);
  auto *subFontLineEdit =
      new QLineEdit(m_viewModel->subtitleViewModel()->subFont(), this);
  subFontLineEdit->setToolTip(SettingsViewModel::getOptionDescription("sub-font"));
  connect(subFontLineEdit, &QLineEdit::textChanged,
          m_viewModel->subtitleViewModel(), &SubtitleViewModel::setSubFont);
  connect(m_viewModel->subtitleViewModel(), &SubtitleViewModel::subFontChanged,
          subFontLineEdit, &QLineEdit::setText);
  subFontLayout->addWidget(subFontLabel);
  subFontLayout->addWidget(subFontLineEdit);
  subtitlesLayout->addLayout(subFontLayout);

  return subtitlesTab;
}

auto SettingsView::createPlaybackTab() -> QWidget * {
  auto *playbackTab = new QWidget(m_tabWidget);
  auto *playbackLayout = new QVBoxLayout(playbackTab);

  // Resume Playback checkbox
  auto *resumePlaybackCheckBox = new QCheckBox("Resume Playback", this);
  resumePlaybackCheckBox->setChecked(
      m_viewModel->playbackBehaviorViewModel()->resumePlayback());
  resumePlaybackCheckBox->setToolTip(
      SettingsViewModel::getOptionDescription("resume-playback"));
  connect(resumePlaybackCheckBox, &QCheckBox::toggled,
          m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::setResumePlayback);
  connect(m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::resumePlaybackChanged,
          resumePlaybackCheckBox, &QCheckBox::setChecked);
  playbackLayout->addWidget(resumePlaybackCheckBox);

  // Save Position On Quit checkbox
  auto *savePositionOnQuitCheckBox =
      new QCheckBox("Save Position On Quit", this);
  savePositionOnQuitCheckBox->setChecked(
      m_viewModel->playbackBehaviorViewModel()->savePositionOnQuit());
  savePositionOnQuitCheckBox->setToolTip(
      SettingsViewModel::getOptionDescription("save-position-on-quit"));
  connect(savePositionOnQuitCheckBox, &QCheckBox::toggled,
          m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::setSavePositionOnQuit);
  connect(m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::savePositionOnQuitChanged,
          savePositionOnQuitCheckBox, &QCheckBox::setChecked);
  playbackLayout->addWidget(savePositionOnQuitCheckBox);

  // Loop File ComboBox
  auto *loopFileLayout = new QHBoxLayout();
  auto *loopFileLabel = new QLabel("Loop File:", this);
  auto *loopFileComboBox = new QComboBox(this);
  loopFileComboBox->addItems({"no", "yes", "inf"});
  loopFileComboBox->setCurrentText(
      m_viewModel->playbackBehaviorViewModel()->loopFile());
  loopFileComboBox->setToolTip(SettingsViewModel::getOptionDescription("loop-file"));
  connect(loopFileComboBox, &QComboBox::currentTextChanged,
          m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::setLoopFile);
  connect(m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::loopFileChanged, loopFileComboBox,
          &QComboBox::setCurrentText);
  loopFileLayout->addWidget(loopFileLabel);
  loopFileLayout->addWidget(loopFileComboBox);
  playbackLayout->addLayout(loopFileLayout);

  // Keep Open ComboBox
  auto *keepOpenLayout = new QHBoxLayout();
  auto *keepOpenLabel = new QLabel("Keep Open:", this);
  auto *keepOpenComboBox = new QComboBox(this);
  keepOpenComboBox->addItems({"no", "yes", "always"});
  keepOpenComboBox->setCurrentText(
      m_viewModel->playbackBehaviorViewModel()->keepOpen());
  keepOpenComboBox->setToolTip(SettingsViewModel::getOptionDescription("keep-open"));
  connect(keepOpenComboBox, &QComboBox::currentTextChanged,
          m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::setKeepOpen);
  connect(m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::keepOpenChanged, keepOpenComboBox,
          &QComboBox::setCurrentText);
  keepOpenLayout->addWidget(keepOpenLabel);
  keepOpenLayout->addWidget(keepOpenComboBox);
  playbackLayout->addLayout(keepOpenLayout);

  // Autofit Larger LineEdit
  auto *autofitLargerLayout = new QHBoxLayout();
  auto *autofitLargerLabel = new QLabel("Autofit Larger:", this);
  auto *autofitLargerLineEdit = new QLineEdit(
      m_viewModel->playbackBehaviorViewModel()->autofitLarger(), this);
  autofitLargerLineEdit->setToolTip(
      SettingsViewModel::getOptionDescription("autofit-larger"));
  connect(autofitLargerLineEdit, &QLineEdit::textChanged,
          m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::setAutofitLarger);
  connect(m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::autofitLargerChanged,
          autofitLargerLineEdit, &QLineEdit::setText);
  autofitLargerLayout->addWidget(autofitLargerLabel);
  autofitLargerLayout->addWidget(autofitLargerLineEdit);
  playbackLayout->addLayout(autofitLargerLayout);

  // YTDL Raw Options LineEdit
  auto *ytdlRawOptionsLayout = new QHBoxLayout();
  auto *ytdlRawOptionsLabel = new QLabel("YTDL Raw Options:", this);
  auto *ytdlRawOptionsLineEdit = new QLineEdit(
      m_viewModel->playbackBehaviorViewModel()->ytdlRawOptions(), this);
  ytdlRawOptionsLineEdit->setToolTip(
      SettingsViewModel::getOptionDescription("ytdl-raw-options"));
  connect(ytdlRawOptionsLineEdit, &QLineEdit::textChanged,
          m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::setYtdlRawOptions);
  connect(m_viewModel->playbackBehaviorViewModel(),
          &PlaybackBehaviorViewModel::ytdlRawOptionsChanged,
          ytdlRawOptionsLineEdit, &QLineEdit::setText);
  ytdlRawOptionsLayout->addWidget(ytdlRawOptionsLabel);
  ytdlRawOptionsLayout->addWidget(ytdlRawOptionsLineEdit);
  playbackLayout->addLayout(ytdlRawOptionsLayout);

  return playbackTab;
}

auto SettingsView::createPerformanceTab() -> QWidget * {
  auto *performanceTab = new QWidget(m_tabWidget);
  auto *performanceLayout = new QVBoxLayout(performanceTab);

  // Cache checkbox
  auto *cacheCheckBox = new QCheckBox("Enable Cache", this);
  cacheCheckBox->setChecked(
      m_viewModel->performanceCachingViewModel()->cache());
  cacheCheckBox->setToolTip(SettingsViewModel::getOptionDescription("cache"));
  connect(cacheCheckBox, &QCheckBox::toggled,
          m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::setCache);
  connect(m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::cacheChanged, cacheCheckBox,
          &QCheckBox::setChecked);
  performanceLayout->addWidget(cacheCheckBox);

  // Cache Seconds SpinBox
  auto *cacheSecsLayout = new QHBoxLayout();
  auto *cacheSecsLabel = new QLabel("Cache Duration (seconds):", this);
  auto *cacheSecsSpinBox = new QSpinBox(this);
  cacheSecsSpinBox->setRange(
      0, MPVDeck::Constants::MAX_CACHE_SECS); // Example range, 0 to 10 minutes
  cacheSecsSpinBox->setValue(
      m_viewModel->performanceCachingViewModel()->cacheSecs());
  cacheSecsSpinBox->setToolTip(SettingsViewModel::getOptionDescription("cache-secs"));
  connect(cacheSecsSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::setCacheSecs);
  connect(m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::cacheSecsChanged, cacheSecsSpinBox,
          &QSpinBox::setValue);
  cacheSecsLayout->addWidget(cacheSecsLabel);
  cacheSecsLayout->addWidget(cacheSecsSpinBox);
  performanceLayout->addLayout(cacheSecsLayout);

  // Demuxer Max Bytes LineEdit
  auto *demuxerMaxBytesLayout = new QHBoxLayout();
  auto *demuxerMaxBytesLabel = new QLabel("Demuxer Max Bytes:", this);
  auto *demuxerMaxBytesLineEdit = new QLineEdit(
      m_viewModel->performanceCachingViewModel()->demuxerMaxBytes(), this);
  demuxerMaxBytesLineEdit->setToolTip(
      SettingsViewModel::getOptionDescription("demuxer-max-bytes"));
  connect(demuxerMaxBytesLineEdit, &QLineEdit::textChanged,
          m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::setDemuxerMaxBytes);
  connect(m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::demuxerMaxBytesChanged,
          demuxerMaxBytesLineEdit, &QLineEdit::setText);
  demuxerMaxBytesLayout->addWidget(demuxerMaxBytesLabel);
  demuxerMaxBytesLayout->addWidget(demuxerMaxBytesLineEdit);
  performanceLayout->addLayout(demuxerMaxBytesLayout);

  // Hardware Decoding (hwdec) ComboBox
  auto *hwdecLayout = new QHBoxLayout();
  auto *hwdecLabel = new QLabel("Hardware Decoding:", this);
  auto *hwdecComboBox = new QComboBox(this);
  hwdecComboBox->addItems({"auto", "auto-safe", "vaapi", "nvdec", "vdpau",
                           "dxva2", "videotoolbox", "none"});
  hwdecComboBox->setCurrentText(
      m_viewModel->performanceCachingViewModel()->hwdec());
  hwdecComboBox->setToolTip(SettingsViewModel::getOptionDescription("hwdec"));
  connect(hwdecComboBox, &QComboBox::currentTextChanged,
          m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::setHwdec);
  connect(m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::hwdecChanged, hwdecComboBox,
          &QComboBox::setCurrentText);
  hwdecLayout->addWidget(hwdecLabel);
  hwdecLayout->addWidget(hwdecComboBox);
  performanceLayout->addLayout(hwdecLayout);

  // Hardware Decoding Codecs (hwdec-codecs) LineEdit
  auto *hwdecCodecsLayout = new QHBoxLayout();
  auto *hwdecCodecsLabel = new QLabel("Hardware Decoding Codecs:", this);
  auto *hwdecCodecsLineEdit = new QLineEdit(
      m_viewModel->performanceCachingViewModel()->hwdecCodecs(), this);
  hwdecCodecsLineEdit->setToolTip(
      SettingsViewModel::getOptionDescription("hwdec-codecs"));
  connect(hwdecCodecsLineEdit, &QLineEdit::textChanged,
          m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::setHwdecCodecs);
  connect(m_viewModel->performanceCachingViewModel(),
          &PerformanceCachingViewModel::hwdecCodecsChanged, hwdecCodecsLineEdit,
          &QLineEdit::setText);
  hwdecCodecsLayout->addWidget(hwdecCodecsLabel);
  hwdecCodecsLayout->addWidget(hwdecCodecsLineEdit);
  performanceLayout->addLayout(hwdecCodecsLayout);

  return performanceTab;
}

auto SettingsView::createInterfaceTab() -> QWidget * {
  auto *interfaceTab = new QWidget(m_tabWidget);
  auto *interfaceLayout = new QVBoxLayout(interfaceTab);

  // OSD Level SpinBox
  auto *osdLevelLayout = new QHBoxLayout();
  auto *osdLevelLabel = new QLabel("OSD Level:", this);
  auto *osdLevelSpinBox = new QSpinBox(this);
  osdLevelSpinBox->setRange(0, MPVDeck::Constants::MAX_OSD_LEVEL);
  osdLevelSpinBox->setValue(m_viewModel->interfaceOsdViewModel()->osdLevel());
  osdLevelSpinBox->setToolTip(SettingsViewModel::getOptionDescription("osd-level"));
  connect(osdLevelSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::setOsdLevel);
  connect(m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::osdLevelChanged, osdLevelSpinBox,
          &QSpinBox::setValue);
  osdLevelLayout->addWidget(osdLevelLabel);
  osdLevelLayout->addWidget(osdLevelSpinBox);
  interfaceLayout->addLayout(osdLevelLayout);

  // OSD Font Size SpinBox
  auto *osdFontSizeLayout = new QHBoxLayout();
  auto *osdFontSizeLabel = new QLabel("OSD Font Size:", this);
  auto *osdFontSizeSpinBox = new QSpinBox(this);
  osdFontSizeSpinBox->setRange(0, MPVDeck::Constants::MAX_OSD_FONT_SIZE); // Example range
  osdFontSizeSpinBox->setValue(
      m_viewModel->interfaceOsdViewModel()->osdFontSize());
  osdFontSizeSpinBox->setToolTip(
      SettingsViewModel::getOptionDescription("osd-font-size"));
  connect(osdFontSizeSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::setOsdFontSize);
  connect(m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::osdFontSizeChanged, osdFontSizeSpinBox,
          &QSpinBox::setValue);
  osdFontSizeLayout->addWidget(osdFontSizeLabel);
  osdFontSizeLayout->addWidget(osdFontSizeSpinBox);
  interfaceLayout->addLayout(osdFontSizeLayout);

  // OSD Duration SpinBox
  auto *osdDurationLayout = new QHBoxLayout();
  auto *osdDurationLabel = new QLabel("OSD Duration (ms):", this);
  auto *osdDurationSpinBox = new QSpinBox(this);
  osdDurationSpinBox->setRange(0, MPVDeck::Constants::MAX_OSD_DURATION); // Example range
  osdDurationSpinBox->setValue(
      m_viewModel->interfaceOsdViewModel()->osdDuration());
  osdDurationSpinBox->setToolTip(
      SettingsViewModel::getOptionDescription("osd-duration"));
  connect(osdDurationSpinBox,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::setOsdDuration);
  connect(m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::osdDurationChanged, osdDurationSpinBox,
          &QSpinBox::setValue);
  osdDurationLayout->addWidget(osdDurationLabel);
  osdDurationLayout->addWidget(osdDurationSpinBox);
  interfaceLayout->addLayout(osdDurationLayout);

  // On-Screen Controller (osc) checkbox
  auto *oscCheckBox = new QCheckBox("On-Screen Controller", this);
  oscCheckBox->setChecked(m_viewModel->interfaceOsdViewModel()->osc());
  oscCheckBox->setToolTip(SettingsViewModel::getOptionDescription("osc"));
  connect(oscCheckBox, &QCheckBox::toggled,
          m_viewModel->interfaceOsdViewModel(), &InterfaceOsdViewModel::setOsc);
  connect(m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::oscChanged, oscCheckBox,
          &QCheckBox::setChecked);
  interfaceLayout->addWidget(oscCheckBox);

  // Screenshot Format ComboBox
  auto *screenshotFormatLayout = new QHBoxLayout();
  auto *screenshotFormatLabel = new QLabel("Screenshot Format:", this);
  auto *screenshotFormatComboBox = new QComboBox(this);
  screenshotFormatComboBox->addItems({"png", "jpg", "bmp"});
  screenshotFormatComboBox->setCurrentText(
      m_viewModel->interfaceOsdViewModel()->screenshotFormat());
  screenshotFormatComboBox->setToolTip(
      SettingsViewModel::getOptionDescription("screenshot-format"));
  connect(screenshotFormatComboBox, &QComboBox::currentTextChanged,
          m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::setScreenshotFormat);
  connect(m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::screenshotFormatChanged,
          screenshotFormatComboBox, &QComboBox::setCurrentText);
  screenshotFormatLayout->addWidget(screenshotFormatLabel);
  screenshotFormatLayout->addWidget(screenshotFormatComboBox);
  interfaceLayout->addLayout(screenshotFormatLayout);

  // Screenshot Directory LineEdit
  auto *screenshotDirectoryLayout = new QHBoxLayout();
  auto *screenshotDirectoryLabel = new QLabel("Screenshot Directory:", this);
  auto *screenshotDirectoryLineEdit = new QLineEdit(
      m_viewModel->interfaceOsdViewModel()->screenshotDirectory(), this);
  screenshotDirectoryLineEdit->setToolTip(
      SettingsViewModel::getOptionDescription("screenshot-directory"));
  connect(screenshotDirectoryLineEdit, &QLineEdit::textChanged,
          m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::setScreenshotDirectory);
  connect(m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::screenshotDirectoryChanged,
          screenshotDirectoryLineEdit, &QLineEdit::setText);
  screenshotDirectoryLayout->addWidget(screenshotDirectoryLabel);
  screenshotDirectoryLayout->addWidget(screenshotDirectoryLineEdit);
  interfaceLayout->addLayout(screenshotDirectoryLayout);

  // Screenshot Template LineEdit
  auto *screenshotTemplateLayout = new QHBoxLayout();
  auto *screenshotTemplateLabel = new QLabel("Screenshot Template:", this);
  auto *screenshotTemplateLineEdit = new QLineEdit(
      m_viewModel->interfaceOsdViewModel()->screenshotTemplate(), this);
  screenshotTemplateLineEdit->setToolTip(
      SettingsViewModel::getOptionDescription("screenshot-template"));
  connect(screenshotTemplateLineEdit, &QLineEdit::textChanged,
          m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::setScreenshotTemplate);
  connect(m_viewModel->interfaceOsdViewModel(),
          &InterfaceOsdViewModel::screenshotTemplateChanged,
          screenshotTemplateLineEdit, &QLineEdit::setText);
  screenshotTemplateLayout->addWidget(screenshotTemplateLabel);
  screenshotTemplateLayout->addWidget(screenshotTemplateLineEdit);
  interfaceLayout->addLayout(screenshotTemplateLayout);

  return interfaceTab;
}

auto SettingsView::createRawConfigTab() -> QWidget * {
  auto *tab = new QWidget(m_tabWidget);
  auto *layout = new QVBoxLayout(tab);

  // Button row
  auto *buttonLayout = new QHBoxLayout();
  auto *refreshBtn = new QPushButton("↻ Refresh from Form", this);
  auto *applyBtn = new QPushButton("⬇ Apply to Form", this);
  auto *exportBtn = new QPushButton("💾 Export...", this);

  buttonLayout->addWidget(refreshBtn);
  buttonLayout->addWidget(applyBtn);
  buttonLayout->addWidget(exportBtn);
  buttonLayout->addStretch();

  // Text editor
  m_rawConfigEditor = new QPlainTextEdit(this);
  m_rawConfigEditor->setPlaceholderText(
      "// Raw mpv.conf editor\n// Use buttons to sync with form or edit "
      "directly");

  // Syntax highlighting (basic)
  QFont font("Monospace", MPVDeck::Constants::DEFAULT_FONT_SIZE);
  m_rawConfigEditor->setFont(font);

  layout->addLayout(buttonLayout);
  layout->addWidget(m_rawConfigEditor);

  // Connect signals
  connect(refreshBtn, &QPushButton::clicked,
          [this]() -> void { refreshRawEditor(m_rawConfigEditor); });

  connect(applyBtn, &QPushButton::clicked,
          [this]() -> void { applyRawConfig(m_rawConfigEditor->toPlainText()); });

  connect(exportBtn, &QPushButton::clicked, [this]() -> void {
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Export mpv.conf"), "mpv.conf",
        tr("MPV Configuration Files (*.conf);;All Files (*)"));
    if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << m_rawConfigEditor->toPlainText();
        file.close();
        QMessageBox::information(
            this, tr("Export Successful"),
            tr("Configuration exported to %1").arg(fileName));
      } else {
        QMessageBox::critical(this, tr("Export Failed"),
                              tr("Could not write to file %1").arg(fileName));
      }
    }
  });

  // Initial load
  refreshRawEditor(m_rawConfigEditor);

  return tab;
}

void SettingsView::refreshRawEditor(QPlainTextEdit *textEdit) {
  QString configContent = m_viewModel->getRawConfig();
  textEdit->setPlainText(configContent);
}

void SettingsView::applyRawConfig(const QString &configText) {
  m_viewModel->applyRawConfig(configText);
  m_viewModel
      ->loadSettings(); // Reload settings from the (now updated) config manager
}

void SettingsView::onSaveButtonClicked() { m_viewModel->saveSettings(); }

void SettingsView::onSettingsSaved(bool success) {
  QMessageBox msgBox;
  if (success) {
    msgBox.setText("Settings saved successfully!");
    msgBox.setIcon(QMessageBox::Information);
    refreshRawEditor(m_rawConfigEditor);
  } else {
    msgBox.setText("Failed to save settings.");
    msgBox.setIcon(QMessageBox::Critical);
  }
  msgBox.exec();
}

void SettingsView::setupMenuBar() {
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  QAction *saveAction = m_fileMenu->addAction(tr("&Save"));
  connect(saveAction, &QAction::triggered, this,
          &SettingsView::onSaveButtonClicked);
  QAction *revertAction = m_fileMenu->addAction(tr("&Revert"));
  connect(revertAction, &QAction::triggered, this,
          &SettingsView::onRevertButtonClicked);
  QAction *loadDefaultsAction = m_fileMenu->addAction(tr("&Load Defaults"));
  connect(loadDefaultsAction, &QAction::triggered, this,
          &SettingsView::onLoadDefaultsButtonClicked);
  m_fileMenu->addSeparator();
  QAction *quitAction = m_fileMenu->addAction(tr("&Quit"));
  connect(quitAction, &QAction::triggered, this, &SettingsView::close);
}

void SettingsView::filterTabs(const QString &searchText) {
  for (int i = 0; i < m_tabWidget->count(); ++i) {
    QWidget *tab = m_tabWidget->widget(i);
    bool match = false;

    // Check if tab title contains search text
    if (m_tabWidget->tabText(i).contains(searchText, Qt::CaseInsensitive)) {
      match = true;
    } else {
      // Search within the tab's content
      if (searchWidgets(tab, searchText)) {
        match = true;
      }
    }
    m_tabWidget->setTabVisible(i, match);
  }
}

// Helper function to recursively search for text in widgets
auto SettingsView::searchWidgets(QWidget *parentWidget,
                                 const QString &searchText) -> bool {
  if (parentWidget == nullptr) {
    return false;
  }

  if (SettingsView::widgetContainsText(parentWidget, searchText)) {
    return true;
  }

  // Recursively search in child widgets
  if (std::ranges::any_of(parentWidget->children(), [&](QObject *child) -> bool {
        if (auto *childWidget = qobject_cast<QWidget *>(child)) {
          return searchWidgets(childWidget, searchText);
        }
        return false;
      })) {
    return true;
  }
  return false;
}

auto SettingsView::widgetContainsText(QWidget *widget, const QString &searchText) -> bool {
  if (widget == nullptr) {
    return false;
  }

  // Check if the widget itself contains the text
  if (auto *label = qobject_cast<QLabel *>(widget)) {
    if (label->text().contains(searchText, Qt::CaseInsensitive)) {
      return true;
    }
  } else if (auto *checkBox = qobject_cast<QCheckBox *>(widget)) {
    if (checkBox->text().contains(searchText, Qt::CaseInsensitive)) {
      return true;
    }
  } else if (auto *pushButton = qobject_cast<QPushButton *>(widget)) {
    if (pushButton->text().contains(searchText, Qt::CaseInsensitive)) {
      return true;
    }
  } else if (auto *lineEdit = qobject_cast<QLineEdit *>(widget)) {
    if (lineEdit->text().contains(searchText, Qt::CaseInsensitive)) {
      return true;
    }
    if (lineEdit->placeholderText().contains(searchText, Qt::CaseInsensitive)) {
      return true;
    }
  } else if (auto *comboBox = qobject_cast<QComboBox *>(widget)) {
    if (comboBox->currentText().contains(searchText, Qt::CaseInsensitive)) {
      return true;
    }
    if (std::ranges::any_of(
            std::views::iota(0, comboBox->count()),
            [&](int index) -> bool {
              return comboBox->itemText(index).contains(searchText,
                                                    Qt::CaseInsensitive);
            })) {
      return true;
    }
  }
  // Also check tooltips for all widgets
  if (widget->toolTip().contains(searchText, Qt::CaseInsensitive)) {
    return true;
  }
  return false;
}

void SettingsView::updateSaveButtonState(bool isDirty) {
  m_saveButton->setEnabled(isDirty);
  m_revertButton->setEnabled(isDirty);
  if (isDirty) {
    m_saveButton->setStyleSheet("background-color: orange;");
    m_revertButton->setStyleSheet(
        "background-color: lightblue;"); // A different color for revert
  } else {
    m_saveButton->setStyleSheet("");   // Reset to default
    m_revertButton->setStyleSheet(""); // Reset to default
  }
}
SettingsView::~SettingsView() = default;