#ifndef MPVDECK_SETTINGSVIEW_H
#define MPVDECK_SETTINGSVIEW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLineEdit>

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
  QMenu *m_fileMenu;
  void setupMenuBar();

  QWidget *createAudioTab();
  QWidget *createVideoTab();
  QWidget *createSubtitlesTab();
  QWidget *createPlaybackTab();
  QWidget *createPerformanceTab();
  QWidget *createInterfaceTab();
  QWidget *createRawConfigTab();
  void refreshRawEditor(QPlainTextEdit *textEdit);
  void applyRawConfig(const QString &configText);

  SettingsViewModel *m_viewModel;
  QTabWidget *m_tabWidget;
  QLineEdit *m_searchEdit;

  void filterTabs(const QString &searchText);
};

#endif // MPVDECK_SETTINGSVIEW_H
