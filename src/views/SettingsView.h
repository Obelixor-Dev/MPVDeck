#ifndef MPVDECK_SETTINGSVIEW_H
#define MPVDECK_SETTINGSVIEW_H

#include "../../src/models/ConfigResult.h"
#include <QLineEdit>
#include <QMainWindow>
#include <QMap>
#include <QPlainTextEdit>
#include <QPushButton>

class SettingsViewModel;
class QTabWidget;
class QTimer;

class SettingsView : public QMainWindow
{
  Q_OBJECT

  public:
    explicit SettingsView(SettingsViewModel * viewModel,
                          QWidget* parent = nullptr);
    ~SettingsView() override;

    void closeEvent(QCloseEvent * event) override;

    void setTestMode(bool testMode)
    {
      m_isTestMode = testMode;
    }

    // Explicitly delete copy and move constructors/assignment operators
    SettingsView(const SettingsView&)            = delete;
    SettingsView& operator=(const SettingsView&) = delete;
    SettingsView(SettingsView&&)                 = delete;
    SettingsView& operator=(SettingsView&&)      = delete;

  private slots:
    auto onSaveButtonClicked() -> void;
    auto onSettingsSaved(const MPVDeck::ConfigResult& result) -> void;
    auto onRevertButtonClicked() -> void;
    auto onLoadDefaultsButtonClicked() -> void;
    auto updateSaveButtonState(bool isDirty) -> void;
    auto onSearchTextChanged(const QString& searchText) -> void;

  signals:
    void warningMessageDisplayed(const QString& message);

  private:
    bool   m_isTestMode = false;
    QMenu* m_fileMenu;
    auto   setupMenuBar() -> void;

    auto createAudioTab() -> QWidget*;
    auto createVideoTab() -> QWidget*;
    auto createSubtitlesTab() -> QWidget*;
    auto createPlaybackTab() -> QWidget*;
    auto createPerformanceTab() -> QWidget*;
    auto createInterfaceTab() -> QWidget*;
    auto createRawConfigTab() -> QWidget*;
    auto refreshRawEditor(QPlainTextEdit * textEdit)->void;
    auto applyRawConfig(const QString& configText) -> void;

    SettingsViewModel* m_viewModel;
    QTabWidget*        m_tabWidget;
    QLineEdit*         m_searchEdit;
    QPlainTextEdit*    m_rawConfigEditor;
    QPushButton*       m_saveButton;
    QPushButton*       m_revertButton;

    QTimer* m_searchTimer; // New member for debouncing search input
    QMap<QWidget*, QStringList>
        m_searchIndex; // Stores searchable text for each widget

    auto filterTabs() -> void;
    auto indexWidget(QWidget * widget, QWidget * tab)->void;
};

#endif // MPVDECK_SETTINGSVIEW_H
