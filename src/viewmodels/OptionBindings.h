#pragma once

#include <QList>
#include <QPointer>
#include <QString>
#include <functional>

class SettingsViewModel; // Forward declaration

struct OptionBinding
{
  QString                             key;
  std::function<void(const QString&)> setter;
  std::function<QString()>            getter;
};

class OptionBindings
{
public:
  static QList<OptionBinding>
  getAllBindings(QPointer<SettingsViewModel> viewModel);
  static QList<OptionBinding>
  getAudioBindings(const SettingsViewModel* viewModel);
  static QList<OptionBinding>
  getVideoBindings(const SettingsViewModel* viewModel);
  // Add other categories as needed
};
