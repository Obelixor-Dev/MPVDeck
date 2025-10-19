#ifndef MPVDECK_SETTINGSVIEW_H
#define MPVDECK_SETTINGSVIEW_H

#include <QMainWindow>

class SettingsViewModel;
class QTabWidget;

class SettingsView : public QMainWindow {
  Q_OBJECT

public:
  explicit SettingsView(SettingsViewModel *viewModel,
                        QWidget *parent = nullptr);
  ~SettingsView() override;

  // Explicitly delete copy and move constructors/assignment operators
  SettingsView(const SettingsView&) = delete;
  SettingsView& operator=(const SettingsView&) = delete;
  SettingsView(SettingsView&&) = delete;
  SettingsView& operator=(SettingsView&&) = delete;

private slots:
  void onSaveButtonClicked();
  void onSettingsSaved(bool success);
  void onRevertButtonClicked();
  void onLoadDefaultsButtonClicked();

private:
  QWidget *createAudioTab();
  QWidget *createVideoTab();
  QWidget *createSubtitlesTab();
  QWidget *createPlaybackTab();
  QWidget *createPerformanceTab();
  QWidget *createInterfaceTab();

  SettingsViewModel *m_viewModel;
  QTabWidget *m_tabWidget;
};

#endif // MPVDECK_SETTINGSVIEW_H
