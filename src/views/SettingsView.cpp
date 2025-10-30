#include "SettingsView.h"
#include "../../src/utils/MpvConfHighlighter.h"
#include "../Constants.h"
#include "../models/ConfigManager.h"
#include "../viewmodels/SettingsViewModel.h"
#include "private/ControlFactory.h"
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
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGui/QRegularExpressionValidator>
#include <algorithm> // For std::any_of
#include <numeric>
#include <ranges>

using namespace MPVDeck::Constants;

SettingsView::SettingsView(SettingsViewModel* viewModel, QWidget* parent)
    : QMainWindow(parent), m_viewModel(viewModel), m_fileMenu(nullptr),
      m_tabWidget(new QTabWidget(this)), m_searchEdit(new QLineEdit(this)),
      m_rawConfigEditor(nullptr), m_saveButton(nullptr),
      m_revertButton(nullptr), m_searchTimer(new QTimer(this))
{

  m_tabWidget->addTab(createAudioTab(), "Audio");
  m_tabWidget->addTab(createVideoTab(), "Video");
  m_tabWidget->addTab(createSubtitlesTab(), "Subtitles");
  m_tabWidget->addTab(createPlaybackTab(), "Playback & Behavior");
  m_tabWidget->addTab(createPerformanceTab(), "Performance");
  m_tabWidget->addTab(createInterfaceTab(), "Interface");
  m_tabWidget->addTab(createRawConfigTab(), "Raw Config");

  // Populate search index after all tabs are created
  for(int i = 0; i < m_tabWidget->count(); ++i)
  {
    indexWidget(m_tabWidget->widget(i), m_tabWidget->widget(i));
  }

  m_saveButton = new QPushButton("Save", this);

  m_revertButton = new QPushButton("Revert", this);

  auto* loadDefaultsButton = new QPushButton("Load Defaults", this);

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

  auto* buttonLayout = new QHBoxLayout();

  buttonLayout->addWidget(m_saveButton);

  buttonLayout->addWidget(m_revertButton);

  buttonLayout->addWidget(loadDefaultsButton);

  auto* centralWidget = new QWidget(this);
  auto* mainLayout    = new QVBoxLayout(centralWidget);

  m_searchEdit->setPlaceholderText("🔍 Search settings...");
  connect(m_searchEdit, &QLineEdit::textChanged, this,
          &SettingsView::onSearchTextChanged);

  m_searchTimer->setSingleShot(true);
  m_searchTimer->setInterval(300); // 300 ms debounce time
  connect(m_searchTimer, &QTimer::timeout, this, &SettingsView::filterTabs);

  mainLayout->addWidget(m_searchEdit);
  mainLayout->addWidget(m_tabWidget);
  mainLayout->addLayout(buttonLayout);

  setCentralWidget(centralWidget);

  setupMenuBar();
}

void SettingsView::onSearchTextChanged(const QString& searchText)
{
  Q_UNUSED(searchText); // The actual search text is retrieved by filterTabs
                        // from m_searchEdit
  m_searchTimer->start();
}

void SettingsView::closeEvent(QCloseEvent* event)
{
  if(m_viewModel->isDirty())
  {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Unsaved Changes",
        "You have unsaved changes. Would you like to save before closing?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if(reply == QMessageBox::Save)
    {
      m_viewModel->saveSettings();
    }
    else if(reply == QMessageBox::Cancel)
    {
      event->ignore();
      return;
    }
  }

  QSettings settings;
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());
  QMainWindow::closeEvent(event);
}

void SettingsView::onRevertButtonClicked()
{
  m_viewModel->loadSettings();
}

void SettingsView::onLoadDefaultsButtonClicked()
{
  m_viewModel->defaultsLoader()->loadDefaults();
}

auto SettingsView::createAudioTab() -> QWidget*
{
  auto* audioTab    = new QWidget(m_tabWidget);
  auto* audioLayout = new QVBoxLayout(audioTab);

  audioLayout->addWidget(ControlFactory::createCheckBox(
      "Mute", m_viewModel->audioViewModel()->mute(),
      SettingsViewModel::getOptionDescription("mute"),
      [this](bool value) { m_viewModel->audioViewModel()->setMute(value); },
      m_viewModel->audioViewModel(), &AudioViewModel::muteChanged, audioTab));

  audioLayout->addWidget(ControlFactory::createSlider(
      "Volume:", m_viewModel->audioViewModel()->volume(), 0,
      MPVDeck::Constants::MAX_VOLUME,
      SettingsViewModel::getOptionDescription("volume"),
      [this](int value) { m_viewModel->audioViewModel()->setVolume(value); },
      m_viewModel->audioViewModel(), &AudioViewModel::volumeChanged, audioTab));

  audioLayout->addWidget(ControlFactory::createComboBox(
      "Audio Device:", m_viewModel->audioViewModel()->audioDevice(),
      AudioViewModel::audioDevices(),
      SettingsViewModel::getOptionDescription("audio-device"),
      [this](const QString& value)
      { m_viewModel->audioViewModel()->setAudioDevice(value); },
      m_viewModel->audioViewModel(), &AudioViewModel::audioDeviceChanged,
      audioTab));

  audioLayout->addWidget(ControlFactory::createCheckBox(
      "Normalize Downmix",
      m_viewModel->audioViewModel()->audioNormalizeDownmix(),
      SettingsViewModel::getOptionDescription("audio-normalize-downmix"),
      [this](bool value)
      { m_viewModel->audioViewModel()->setAudioNormalizeDownmix(value); },
      m_viewModel->audioViewModel(),
      &AudioViewModel::audioNormalizeDownmixChanged, audioTab));

  audioLayout->addWidget(ControlFactory::createComboBox(
      "Audio Channels:", m_viewModel->audioViewModel()->audioChannels(),
      {"stereo", "mono", "5.1", "7.1", "downmix"},
      SettingsViewModel::getOptionDescription("audio-channels"),
      [this](const QString& value)
      { m_viewModel->audioViewModel()->setAudioChannels(value); },
      m_viewModel->audioViewModel(), &AudioViewModel::audioChannelsChanged,
      audioTab));

  audioLayout->addWidget(ControlFactory::createDoubleSpinBox(
      "Audio Delay:", m_viewModel->audioViewModel()->audioDelay(),
      MPVDeck::Constants::MIN_AUDIO_DELAY, MPVDeck::Constants::MAX_AUDIO_DELAY,
      MPVDeck::Constants::AUDIO_DELAY_SINGLE_STEP,
      SettingsViewModel::getOptionDescription("audio-delay"),
      [this](double value)
      { m_viewModel->audioViewModel()->setAudioDelay(value); },
      m_viewModel->audioViewModel(), &AudioViewModel::audioDelayChanged,
      audioTab));

  return audioTab;
}

auto SettingsView::createVideoTab() -> QWidget*
{
  auto* videoTab    = new QWidget(m_tabWidget);
  auto* videoLayout = new QVBoxLayout(videoTab);

  videoLayout->addWidget(ControlFactory::createComboBox(
      "Profile:", m_viewModel->videoViewModel()->videoProfile(),
      VideoViewModel::videoProfiles(),
      SettingsViewModel::getOptionDescription("profile"),
      [this](const QString& value)
      { m_viewModel->videoViewModel()->setVideoProfile(value); },
      m_viewModel->videoViewModel(), &VideoViewModel::videoProfileChanged,
      videoTab));

  videoLayout->addWidget(ControlFactory::createComboBox(
      "Scale:", m_viewModel->videoViewModel()->videoScale(),
      VideoViewModel::videoScales(),
      SettingsViewModel::getOptionDescription("scale"),
      [this](const QString& value)
      { m_viewModel->videoViewModel()->setVideoScale(value); },
      m_viewModel->videoViewModel(), &VideoViewModel::videoScaleChanged,
      videoTab));

  videoLayout->addWidget(ControlFactory::createCheckBox(
      "Deband", m_viewModel->videoViewModel()->videoDeband(),
      SettingsViewModel::getOptionDescription("deband"), [this](bool value)
      { m_viewModel->videoViewModel()->setVideoDeband(value); },
      m_viewModel->videoViewModel(), &VideoViewModel::videoDebandChanged,
      videoTab));

  videoLayout->addWidget(ControlFactory::createComboBox(
      "Video Output:", m_viewModel->videoViewModel()->videoVo(),
      {"gpu", "gpu-next", "x11", "xv", "opengl", "sdl"},
      SettingsViewModel::getOptionDescription("vo"),
      [this](const QString& value)
      { m_viewModel->videoViewModel()->setVideoVo(value); },
      m_viewModel->videoViewModel(), &VideoViewModel::videoVoChanged,
      videoTab));

  videoLayout->addWidget(ControlFactory::createComboBox(
      "Chroma Upscaling:", m_viewModel->videoViewModel()->videoCscale(),
      VideoViewModel::videoCscales(),
      SettingsViewModel::getOptionDescription("cscale"),
      [this](const QString& value)
      { m_viewModel->videoViewModel()->setVideoCscale(value); },
      m_viewModel->videoViewModel(), &VideoViewModel::videoCscaleChanged,
      videoTab));

  videoLayout->addWidget(ControlFactory::createComboBox(
      "Downscaling:", m_viewModel->videoViewModel()->videoDscale(),
      VideoViewModel::videoDscales(),
      SettingsViewModel::getOptionDescription("dscale"),
      [this](const QString& value)
      { m_viewModel->videoViewModel()->setVideoDscale(value); },
      m_viewModel->videoViewModel(), &VideoViewModel::videoDscaleChanged,
      videoTab));

  videoLayout->addWidget(ControlFactory::createCheckBox(
      "Interpolation", m_viewModel->videoViewModel()->videoInterpolation(),
      SettingsViewModel::getOptionDescription("interpolation"),
      [this](bool value)
      { m_viewModel->videoViewModel()->setVideoInterpolation(value); },
      m_viewModel->videoViewModel(), &VideoViewModel::videoInterpolationChanged,
      videoTab));

  videoLayout->addWidget(ControlFactory::createComboBox(
      "Temporal Scaling:", m_viewModel->videoViewModel()->videoTscale(),
      VideoViewModel::videoTscales(),
      SettingsViewModel::getOptionDescription("tscale"),
      [this](const QString& value)
      { m_viewModel->videoViewModel()->setVideoTscale(value); },
      m_viewModel->videoViewModel(), &VideoViewModel::videoTscaleChanged,
      videoTab));

  videoLayout->addWidget(ControlFactory::createComboBox(
      "Video Sync:", m_viewModel->videoViewModel()->videoVideoSync(),
      InterfaceOsdViewModel::videoSyncOptions(),
      SettingsViewModel::getOptionDescription("video-sync"),
      [this](const QString& value)
      { m_viewModel->videoViewModel()->setVideoVideoSync(value); },
      m_viewModel->videoViewModel(), &VideoViewModel::videoVideoSyncChanged,
      videoTab));

  return videoTab;
}

auto SettingsView::createSubtitlesTab() -> QWidget*
{
  auto* subtitlesTab    = new QWidget(m_tabWidget);
  auto* subtitlesLayout = new QVBoxLayout(subtitlesTab);

  subtitlesLayout->addWidget(ControlFactory::createCheckBox(
      "Visible", m_viewModel->subtitleViewModel()->subtitleVisibility(),
      SettingsViewModel::getOptionDescription("sub-visibility"),
      [this](bool value)
      { m_viewModel->subtitleViewModel()->setSubtitleVisibility(value); },
      m_viewModel->subtitleViewModel(),
      &SubtitleViewModel::subtitleVisibilityChanged, subtitlesTab));

  subtitlesLayout->addWidget(ControlFactory::createSpinBox(
      "Font Size:", m_viewModel->subtitleViewModel()->subtitleFontSize(),
      MPVDeck::Constants::MIN_SUBTITLE_FONT_SIZE,
      MPVDeck::Constants::MAX_SUBTITLE_FONT_SIZE,
      SettingsViewModel::getOptionDescription("sub-font-size"),
      [this](int value)
      { m_viewModel->subtitleViewModel()->setSubtitleFontSize(value); },
      m_viewModel->subtitleViewModel(),
      &SubtitleViewModel::subtitleFontSizeChanged, subtitlesTab));

  // Color
  auto* subColorLayout = new QHBoxLayout();
  auto* subColorLabel  = new QLabel("Color:", this);
  auto* subColorLineEdit =
      new QLineEdit(m_viewModel->subtitleViewModel()->subtitleColor(),
                    subtitlesTab);
  subColorLineEdit->setObjectName("subtitleColorLineEdit");
  subColorLineEdit->setToolTip(
      SettingsViewModel::getOptionDescription("sub-color"));
  subColorLineEdit->setValidator(nullptr);
  auto* subColorButton = new QPushButton("Pick Color", subtitlesTab);
  connect(subColorButton, &QPushButton::clicked,
          [this, subColorLineEdit]() -> void
          {
            QColor color = QColorDialog::getColor(
                QColor(m_viewModel->subtitleViewModel()->subtitleColor()), this,
                "Pick a color");
            if(color.isValid())
            {
              m_viewModel->subtitleViewModel()->setSubtitleColor(color.name());
            }
          });
  connect(subColorLineEdit, &QLineEdit::textChanged,
          m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::setSubtitleColor);
  connect(m_viewModel->subtitleViewModel(),
          &SubtitleViewModel::subtitleColorChanged, subColorLineEdit,
          &QLineEdit::setText);
  connect(m_viewModel->subtitleViewModel(), &SubtitleViewModel::errorOccurred,
          this,
          [this](const QString& message)
          {
            if(!m_isTestMode)
            {
              QMessageBox::warning(this, "Input Error", message);
            }
            emit warningMessageDisplayed(message); // Emit the signal here
          });
  subColorLayout->addWidget(subColorLabel);
  subColorLayout->addWidget(subColorLineEdit);
  subColorLayout->addWidget(subColorButton);
  subtitlesLayout->addLayout(subColorLayout);

  subtitlesLayout->addWidget(ControlFactory::createLineEdit(
      "Subtitle Languages:",
      m_viewModel->subtitleViewModel()->subtitleLanguages(),
      SettingsViewModel::getOptionDescription("slang"),
      [this](const QString& value)
      { m_viewModel->subtitleViewModel()->setSubtitleLanguages(value); },
      m_viewModel->subtitleViewModel(),
      &SubtitleViewModel::subtitleLanguagesChanged, subtitlesTab));

  subtitlesLayout->addWidget(ControlFactory::createComboBox(
      "Auto Load Subtitles:", m_viewModel->subtitleViewModel()->subAuto(),
      m_viewModel->subtitleViewModel()->subAutoOptions(),
      SettingsViewModel::getOptionDescription("sub-auto"),
      [this](const QString& value)
      { m_viewModel->subtitleViewModel()->setSubAuto(value); },
      m_viewModel->subtitleViewModel(), &SubtitleViewModel::subAutoChanged,
      subtitlesTab));

  subtitlesLayout->addWidget(ControlFactory::createLineEdit(
      "Subtitle ID:", m_viewModel->subtitleViewModel()->sid(),
      SettingsViewModel::getOptionDescription("sid"),
      [this](const QString& value)
      { m_viewModel->subtitleViewModel()->setSid(value); },
      m_viewModel->subtitleViewModel(), &SubtitleViewModel::sidChanged,
      subtitlesTab));

  subtitlesLayout->addWidget(ControlFactory::createCheckBox(
      "Show Forced Subtitles Only",
      m_viewModel->subtitleViewModel()->subForcedOnly(),
      SettingsViewModel::getOptionDescription("sub-forced-only"),
      [this](bool value)
      { m_viewModel->subtitleViewModel()->setSubForcedOnly(value); },
      m_viewModel->subtitleViewModel(),
      &SubtitleViewModel::subForcedOnlyChanged, subtitlesTab));

  subtitlesLayout->addWidget(ControlFactory::createLineEdit(
      "Subtitle Font:", m_viewModel->subtitleViewModel()->subFont(),
      SettingsViewModel::getOptionDescription("sub-font"),
      [this](const QString& value)
      { m_viewModel->subtitleViewModel()->setSubFont(value); },
      m_viewModel->subtitleViewModel(), &SubtitleViewModel::subFontChanged,
      subtitlesTab));

  return subtitlesTab;
}

auto SettingsView::createPlaybackTab() -> QWidget*
{
  auto* playbackTab    = new QWidget(m_tabWidget);
  auto* playbackLayout = new QVBoxLayout(playbackTab);

  playbackLayout->addWidget(ControlFactory::createCheckBox(
      "Resume Playback",
      m_viewModel->playbackBehaviorViewModel()->resumePlayback(),
      SettingsViewModel::getOptionDescription("resume-playback"),
      [this](bool value)
      { m_viewModel->playbackBehaviorViewModel()->setResumePlayback(value); },
      m_viewModel->playbackBehaviorViewModel(),
      &PlaybackBehaviorViewModel::resumePlaybackChanged, playbackTab));

  playbackLayout->addWidget(ControlFactory::createCheckBox(
      "Save Position On Quit",
      m_viewModel->playbackBehaviorViewModel()->savePositionOnQuit(),
      SettingsViewModel::getOptionDescription("save-position-on-quit"),
      [this](bool value)
      {
        m_viewModel->playbackBehaviorViewModel()->setSavePositionOnQuit(value);
      },
      m_viewModel->playbackBehaviorViewModel(),
      &PlaybackBehaviorViewModel::savePositionOnQuitChanged, playbackTab));

  playbackLayout->addWidget(ControlFactory::createComboBox(
      "Loop File:", m_viewModel->playbackBehaviorViewModel()->loopFile(),
      m_viewModel->playbackBehaviorViewModel()->loopFileOptions(),
      SettingsViewModel::getOptionDescription("loop-file"),
      [this](const QString& value)
      { m_viewModel->playbackBehaviorViewModel()->setLoopFile(value); },
      m_viewModel->playbackBehaviorViewModel(),
      &PlaybackBehaviorViewModel::loopFileChanged, playbackTab));

  playbackLayout->addWidget(ControlFactory::createComboBox(
      "Keep Open:", m_viewModel->playbackBehaviorViewModel()->keepOpen(),
      m_viewModel->playbackBehaviorViewModel()->keepOpenOptions(),
      SettingsViewModel::getOptionDescription("keep-open"),
      [this](const QString& value)
      { m_viewModel->playbackBehaviorViewModel()->setKeepOpen(value); },
      m_viewModel->playbackBehaviorViewModel(),
      &PlaybackBehaviorViewModel::keepOpenChanged, playbackTab));

  playbackLayout->addWidget(ControlFactory::createLineEdit(
      "Autofit Larger:",
      m_viewModel->playbackBehaviorViewModel()->autofitLarger(),
      SettingsViewModel::getOptionDescription("autofit-larger"),
      [this](const QString& value)
      { m_viewModel->playbackBehaviorViewModel()->setAutofitLarger(value); },
      m_viewModel->playbackBehaviorViewModel(),
      &PlaybackBehaviorViewModel::autofitLargerChanged, playbackTab));

  playbackLayout->addWidget(ControlFactory::createLineEdit(
      "YTDL Raw Options:",
      m_viewModel->playbackBehaviorViewModel()->ytdlRawOptions(),
      SettingsViewModel::getOptionDescription("ytdl-raw-options"),
      [this](const QString& value)
      { m_viewModel->playbackBehaviorViewModel()->setYtdlRawOptions(value); },
      m_viewModel->playbackBehaviorViewModel(),
      &PlaybackBehaviorViewModel::ytdlRawOptionsChanged, playbackTab));

  return playbackTab;
}

auto SettingsView::createPerformanceTab() -> QWidget*
{
  auto* performanceTab    = new QWidget(m_tabWidget);
  auto* performanceLayout = new QVBoxLayout(performanceTab);

  performanceLayout->addWidget(ControlFactory::createCheckBox(
      "Enable Cache", m_viewModel->performanceCachingViewModel()->cache(),
      SettingsViewModel::getOptionDescription("cache"), [this](bool value)
      { m_viewModel->performanceCachingViewModel()->setCache(value); },
      m_viewModel->performanceCachingViewModel(),
      &PerformanceCachingViewModel::cacheChanged, performanceTab));

  performanceLayout->addWidget(ControlFactory::createSpinBox(
      "Cache Duration (seconds):",
      m_viewModel->performanceCachingViewModel()->cacheSecs(), 0,
      MPVDeck::Constants::MAX_CACHE_SECS,
      SettingsViewModel::getOptionDescription("cache-secs"), [this](int value)
      { m_viewModel->performanceCachingViewModel()->setCacheSecs(value); },
      m_viewModel->performanceCachingViewModel(),
      &PerformanceCachingViewModel::cacheSecsChanged, performanceTab));

  performanceLayout->addWidget(ControlFactory::createLineEdit(
      "Demuxer Max Bytes:",
      m_viewModel->performanceCachingViewModel()->demuxerMaxBytes(),
      SettingsViewModel::getOptionDescription("demuxer-max-bytes"),
      [this](const QString& value)
      {
        m_viewModel->performanceCachingViewModel()->setDemuxerMaxBytes(value);
      },
      m_viewModel->performanceCachingViewModel(),
      &PerformanceCachingViewModel::demuxerMaxBytesChanged, performanceTab));

  performanceLayout->addWidget(ControlFactory::createComboBox(
      "Hardware Decoding:", m_viewModel->performanceCachingViewModel()->hwdec(),
      m_viewModel->performanceCachingViewModel()->hwdecOptions(),
      SettingsViewModel::getOptionDescription("hwdec"),
      [this](const QString& value)
      { m_viewModel->performanceCachingViewModel()->setHwdec(value); },
      m_viewModel->performanceCachingViewModel(),
      &PerformanceCachingViewModel::hwdecChanged, performanceTab));

  performanceLayout->addWidget(ControlFactory::createLineEdit(
      "Hardware Decoding Codecs:",
      m_viewModel->performanceCachingViewModel()->hwdecCodecs(),
      SettingsViewModel::getOptionDescription("hwdec-codecs"),
      [this](const QString& value)
      { m_viewModel->performanceCachingViewModel()->setHwdecCodecs(value); },
      m_viewModel->performanceCachingViewModel(),
      &PerformanceCachingViewModel::hwdecCodecsChanged, performanceTab));

  return performanceTab;
}

auto SettingsView::createInterfaceTab() -> QWidget*
{
  auto* interfaceTab    = new QWidget(m_tabWidget);
  auto* interfaceLayout = new QVBoxLayout(interfaceTab);

  interfaceLayout->addWidget(ControlFactory::createSpinBox(
      "OSD Level:", m_viewModel->interfaceOsdViewModel()->osdLevel(),
      MPVDeck::Constants::MIN_OSD_LEVEL, MPVDeck::Constants::MAX_OSD_LEVEL,
      SettingsViewModel::getOptionDescription("osd-level"), [this](int value)
      { m_viewModel->interfaceOsdViewModel()->setOsdLevel(value); },
      m_viewModel->interfaceOsdViewModel(),
      &InterfaceOsdViewModel::osdLevelChanged, interfaceTab));

  interfaceLayout->addWidget(ControlFactory::createSpinBox(
      "OSD Font Size:", m_viewModel->interfaceOsdViewModel()->osdFontSize(),
      MPVDeck::Constants::MIN_OSD_FONT_SIZE,
      MPVDeck::Constants::MAX_OSD_FONT_SIZE,
      SettingsViewModel::getOptionDescription("osd-font-size"),
      [this](int value)
      { m_viewModel->interfaceOsdViewModel()->setOsdFontSize(value); },
      m_viewModel->interfaceOsdViewModel(),
      &InterfaceOsdViewModel::osdFontSizeChanged, interfaceTab));

  interfaceLayout->addWidget(ControlFactory::createSpinBox(
      "OSD Duration (ms):", m_viewModel->interfaceOsdViewModel()->osdDuration(),
      MPVDeck::Constants::MIN_OSD_DURATION,
      MPVDeck::Constants::MAX_OSD_DURATION,
      SettingsViewModel::getOptionDescription("osd-duration"), [this](int value)
      { m_viewModel->interfaceOsdViewModel()->setOsdDuration(value); },
      m_viewModel->interfaceOsdViewModel(),
      &InterfaceOsdViewModel::osdDurationChanged, interfaceTab));

  interfaceLayout->addWidget(ControlFactory::createCheckBox(
      "On-Screen Controller", m_viewModel->interfaceOsdViewModel()->osc(),
      SettingsViewModel::getOptionDescription("osc"), [this](bool value)
      { m_viewModel->interfaceOsdViewModel()->setOsc(value); },
      m_viewModel->interfaceOsdViewModel(), &InterfaceOsdViewModel::oscChanged,
      interfaceTab));

  interfaceLayout->addWidget(ControlFactory::createComboBox(
      "Screenshot Format:",
      m_viewModel->interfaceOsdViewModel()->screenshotFormat(),
      m_viewModel->interfaceOsdViewModel()->screenshotFormatOptions(),
      SettingsViewModel::getOptionDescription("screenshot-format"),
      [this](const QString& value)
      { m_viewModel->interfaceOsdViewModel()->setScreenshotFormat(value); },
      m_viewModel->interfaceOsdViewModel(),
      &InterfaceOsdViewModel::screenshotFormatChanged, interfaceTab));

  interfaceLayout->addWidget(ControlFactory::createLineEdit(
      "Screenshot Directory:",
      m_viewModel->interfaceOsdViewModel()->screenshotDirectory(),
      SettingsViewModel::getOptionDescription("screenshot-directory"),
      [this](const QString& value)
      { m_viewModel->interfaceOsdViewModel()->setScreenshotDirectory(value); },
      m_viewModel->interfaceOsdViewModel(),
      &InterfaceOsdViewModel::screenshotDirectoryChanged, interfaceTab));

  interfaceLayout->addWidget(ControlFactory::createLineEdit(
      "Screenshot Template:",
      m_viewModel->interfaceOsdViewModel()->screenshotTemplate(),
      SettingsViewModel::getOptionDescription("screenshot-template"),
      [this](const QString& value)
      { m_viewModel->interfaceOsdViewModel()->setScreenshotTemplate(value); },
      m_viewModel->interfaceOsdViewModel(),
      &InterfaceOsdViewModel::screenshotTemplateChanged, interfaceTab));

  return interfaceTab;
}

auto SettingsView::createRawConfigTab() -> QWidget*
{
  auto* tab    = new QWidget(m_tabWidget);
  auto* layout = new QVBoxLayout(tab);

  // Button row
  auto* buttonLayout = new QHBoxLayout();
  auto* refreshBtn   = new QPushButton("↻ Refresh from Form", tab);
  auto* applyBtn     = new QPushButton("⬇ Apply to Form", tab);
  auto* exportBtn    = new QPushButton("💾 Export...", tab);

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
  new MpvConfHighlighter(m_rawConfigEditor->document()); // Set highlighter

  layout->addLayout(buttonLayout);
  layout->addWidget(m_rawConfigEditor);

  // Connect signals
  connect(refreshBtn, &QPushButton::clicked,
          [this]() -> void { refreshRawEditor(m_rawConfigEditor); });

  connect(applyBtn, &QPushButton::clicked, [this]() -> void
          { applyRawConfig(m_rawConfigEditor->toPlainText()); });

  connect(exportBtn, &QPushButton::clicked,
          [this]() -> void
          {
            QString fileName = QFileDialog::getSaveFileName(
                this, tr("Export mpv.conf"), "mpv.conf",
                tr("MPV Configuration Files (*.conf);;All Files (*)"));
            if(!fileName.isEmpty())
            {
              QFile file(fileName);
              if(file.open(QIODevice::WriteOnly | QIODevice::Text))
              {
                QTextStream out(&file);
                out << m_rawConfigEditor->toPlainText();
                file.close();
                QMessageBox::information(
                    this, tr("Export Successful"),
                    tr("Configuration exported to %1").arg(fileName));
              }
              else
              {
                QMessageBox::critical(
                    this, tr("Export Failed"),
                    tr("Could not write to file %1").arg(fileName));
              }
            }
          });

  // Initial load
  refreshRawEditor(m_rawConfigEditor);

  return tab;
}

void SettingsView::refreshRawEditor(QPlainTextEdit* textEdit)
{
  QString configContent = m_viewModel->configManager()->getRawConfig();
  textEdit->setPlainText(configContent);
}

void SettingsView::applyRawConfig(const QString& configText)
{
  auto result = m_viewModel->configManager()->parseRawConfig(configText);
  if(!result.success)
  {
    QMessageBox::warning(this, "Invalid Configuration", result.errorMessage);
    return;
  }
  m_viewModel
      ->loadSettings(); // Reload settings from the (now updated) config manager
}

void SettingsView::onSaveButtonClicked()
{
  m_viewModel->saveSettings();
}

void SettingsView::onSettingsSaved(const MPVDeck::ConfigResult& result)
{
  QMessageBox msgBox;
  if(result.success)
  {
    msgBox.setText("Settings saved successfully!");
    msgBox.setIcon(QMessageBox::Information);
    refreshRawEditor(m_rawConfigEditor);
  }
  else
  {
    msgBox.setText("Failed to save settings:\n" + result.errorMessage);
    msgBox.setIcon(QMessageBox::Critical);
  }
  msgBox.exec();
}

void SettingsView::setupMenuBar()
{
  m_fileMenu          = menuBar()->addMenu(tr("&File"));
  QAction* saveAction = m_fileMenu->addAction(tr("&Save"));
  connect(saveAction, &QAction::triggered, this,
          &SettingsView::onSaveButtonClicked);
  QAction* revertAction = m_fileMenu->addAction(tr("&Revert"));
  connect(revertAction, &QAction::triggered, this,
          &SettingsView::onRevertButtonClicked);
  QAction* loadDefaultsAction = m_fileMenu->addAction(tr("&Load Defaults"));
  connect(loadDefaultsAction, &QAction::triggered, this,
          &SettingsView::onLoadDefaultsButtonClicked);
  m_fileMenu->addSeparator();
  QAction* quitAction = m_fileMenu->addAction(tr("&Quit"));
  connect(quitAction, &QAction::triggered, this, &SettingsView::close);
}

void SettingsView::filterTabs()
{
  const QString searchText = m_searchEdit->text();

  for(int i = 0; i < m_tabWidget->count(); ++i)
  {
    QWidget* tab   = m_tabWidget->widget(i);
    bool     match = false;

    // Check if tab title contains search text
    if(m_tabWidget->tabText(i).contains(searchText, Qt::CaseInsensitive))
    {
      match = true;
    }
    else
    {
      // Search within the indexed content of the tab
      if(m_searchIndex.contains(tab))
      {
        match = std::any_of(
            m_searchIndex.value(tab).begin(), m_searchIndex.value(tab).end(),
            [&](const QString& indexedText)
            { return indexedText.contains(searchText, Qt::CaseInsensitive); });
      }
    }
    m_tabWidget->setTabVisible(i, match);
  }
}

auto SettingsView::indexWidget(QWidget* widget, QWidget* tab) -> void
{
  if(widget == nullptr)
  {
    return;
  }

  QStringList searchableTexts;

  if(auto* label = qobject_cast<QLabel*>(widget))
  {
    searchableTexts << label->text();
  }
  else if(auto* checkBox = qobject_cast<QCheckBox*>(widget))
  {
    searchableTexts << checkBox->text();
  }
  else if(auto* pushButton = qobject_cast<QPushButton*>(widget))
  {
    searchableTexts << pushButton->text();
  }
  else if(auto* lineEdit = qobject_cast<QLineEdit*>(widget))
  {
    searchableTexts << lineEdit->text() << lineEdit->placeholderText();
  }
  else if(auto* comboBox = qobject_cast<QComboBox*>(widget))
  {
    searchableTexts << comboBox->currentText();
    for(int i = 0; i < comboBox->count(); ++i)
    {
      searchableTexts << comboBox->itemText(i);
    }
  }

  // Add tooltip text for all widgets
  searchableTexts << widget->toolTip();

  // Remove empty strings and duplicates
  searchableTexts.removeDuplicates();
  searchableTexts.removeAll(QString());

  if(!searchableTexts.isEmpty())
  {
    m_searchIndex[tab].append(searchableTexts);
  }

  // Recursively index child widgets
  for(QObject* child : widget->children())
  {
    if(auto* childWidget = qobject_cast<QWidget*>(child))
    {
      indexWidget(childWidget, tab);
    }
  }
}

void SettingsView::updateSaveButtonState(bool isDirty)
{
  m_saveButton->setEnabled(isDirty);
  m_revertButton->setEnabled(isDirty);
  if(isDirty)
  {
    m_saveButton->setStyleSheet("background-color: orange;");
    m_revertButton->setStyleSheet(
        "background-color: lightblue;"); // A different color for revert
  }
  else
  {
    m_saveButton->setStyleSheet("");   // Reset to default
    m_revertButton->setStyleSheet(""); // Reset to default
  }
}
SettingsView::~SettingsView()
{
  disconnect(m_viewModel, &SettingsViewModel::settingsSaved, this,
             &SettingsView::onSettingsSaved);
  disconnect(m_viewModel, &SettingsViewModel::isDirtyChanged, this,
             &SettingsView::updateSaveButtonState);
}