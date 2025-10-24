#pragma once

#include <QJsonDocument>
#include <QObject>
#include <QPointer>

class SettingsViewModel; // Forward declaration

class DefaultsLoader : public QObject
{
  Q_OBJECT
  public:
    explicit DefaultsLoader(SettingsViewModel * viewModel,
                            QObject* parent = nullptr);

    void loadDefaults();

  private:
    QJsonDocument               loadDefaultsJson();
    QPointer<SettingsViewModel> m_viewModel;
};
