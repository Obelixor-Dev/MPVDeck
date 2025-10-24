#ifndef MPVDECKAPPLICATION_H
#define MPVDECKAPPLICATION_H

#include <QApplication>
#include <memory>

class ConfigManager;
class SettingsViewModel;
class SettingsView;

class MPVDeckApplication : public QApplication
{
  Q_OBJECT
  public:
    explicit MPVDeckApplication(int& argc, char** argv);
    ~MPVDeckApplication() override;

    // Explicitly delete copy and move constructors/assignment operators
    MPVDeckApplication(const MPVDeckApplication&)            = delete;
    MPVDeckApplication& operator=(const MPVDeckApplication&) = delete;
    MPVDeckApplication(MPVDeckApplication&&)                 = delete;
    MPVDeckApplication& operator=(MPVDeckApplication&&)      = delete;
    auto                run()
        -> int; // NOLINT(cppcoreguidelines-explicit-virtual-functions,modernize-use-override)

  private:
    std::unique_ptr<ConfigManager>     m_configManager;
    std::unique_ptr<SettingsViewModel> m_settingsViewModel;
    std::unique_ptr<SettingsView>      m_settingsView;

    static auto setupMessageHandlers() -> void;
    auto        setupConfigAndUI()
        -> void; // NOLINT(modernize-use-trailing-return-type)
    auto restoreWindowState()
        -> void; // NOLINT(modernize-use-trailing-return-type)
};

#endif // MPVDECKAPPLICATION_H
