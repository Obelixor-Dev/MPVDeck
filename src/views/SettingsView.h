#ifndef MPVDECK_SETTINGSVIEW_H
#define MPVDECK_SETTINGSVIEW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>

class SettingsViewModel;
class QTabWidget;

class SettingsView : public QMainWindow {
  Q_OBJECT

public:
  explicit SettingsView(SettingsViewModel *viewModel,
                        QWidget *parent = nullptr);
  ~SettingsView() override;

  void closeEvent(QCloseEvent *event) override;

  // Explicitly delete copy and move constructors/assignment operators
  SettingsView(const SettingsView &) = delete;
  SettingsView &operator=(const SettingsView &) = delete;
  SettingsView(SettingsView &&) = delete;
  SettingsView &operator=(SettingsView &&) = delete;

private slots:
  auto onSaveButtonClicked() -> void;
  auto onSettingsSaved(bool success) -> void;
  auto onRevertButtonClicked() -> void;
  auto onLoadDefaultsButtonClicked() -> void;
  auto updateSaveButtonState(bool isDirty) -> void;

private: // NOLINT(readability-redundant-access-specifiers)
  QMenu *m_fileMenu;
  auto setupMenuBar() -> void;

  auto createAudioTab() -> QWidget *;
  auto createVideoTab() -> QWidget *;
  auto createSubtitlesTab() -> QWidget *;
  auto createPlaybackTab() -> QWidget *;
  auto createPerformanceTab() -> QWidget *;
  auto createInterfaceTab() -> QWidget *;
  auto createRawConfigTab() -> QWidget *;
  auto refreshRawEditor(QPlainTextEdit *textEdit) -> void;
  auto applyRawConfig(const QString &configText) -> void;

  SettingsViewModel *m_viewModel;
  QTabWidget *m_tabWidget;
  QLineEdit *m_searchEdit;
  QPlainTextEdit *m_rawConfigEditor;
  QPushButton *m_saveButton;
  QPushButton *m_revertButton;

  auto filterTabs(const QString &searchText) -> void;
  auto searchWidgets(QWidget *parentWidget, const QString &searchText) -> bool;
  static bool widgetContainsText(QWidget *widget, const QString &searchText);
};

#endif // MPVDECK_SETTINGSVIEW_H
