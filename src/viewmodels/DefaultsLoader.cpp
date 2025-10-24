#include "DefaultsLoader.h"
#include "OptionBindings.h"
#include "SettingsViewModel.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory> // New include
#include <algorithm>        // For std::find_if

Q_LOGGING_CATEGORY(lcDefaultsLoader,
                   "mpvdeck.defaultsloader") // New logging category

DefaultsLoader::DefaultsLoader(SettingsViewModel* viewModel, QObject* parent)
    : QObject(parent), m_viewModel(viewModel)
{
}

void DefaultsLoader::loadDefaults()
{
  if(m_viewModel.isNull())
  {
    qCWarning(lcDefaultsLoader)
        << "DefaultsLoader: ViewModel is null. Cannot load defaults.";
    return;
  }

  QJsonDocument doc = loadDefaultsJson();
  if(doc.isNull() || !doc.isObject())
  {
    qCWarning(lcDefaultsLoader) << "Invalid defaults.json document.";
    return;
  }

  QJsonObject obj = doc.object();
  if(!obj.contains("options") || !obj["options"].isArray())
  {
    qCWarning(lcDefaultsLoader)
        << "defaults.json does not contain an 'options' array.";
    return;
  }

  QJsonArray optionsArray = obj["options"].toArray();
  const auto bindings     = OptionBindings::getAllBindings(m_viewModel);

  for(const auto& value : optionsArray)
  {
    if(value.isObject())
    {
      QJsonObject optionObj    = value.toObject();
      QString     name         = optionObj["name"].toString();
      QString     defaultValue = optionObj["default"].toString();

      auto it =
          std::find_if(bindings.begin(), bindings.end(),
                       [&](const OptionBinding& b) { return b.key == name; });
      if(it != bindings.end())
      {
        it->setter(defaultValue);
      }
    }
  }
  if(!m_viewModel.isNull())
  {
    m_viewModel->setIsDirty(
        false); // Loading defaults should not mark as dirty initially
  }
}

QJsonDocument DefaultsLoader::loadDefaultsJson()
{
  QFile file(":/defaults.json"); // Assuming defaults.json is in Qt resources
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qCWarning(lcDefaultsLoader)
        << "Failed to open defaults.json:" << file.errorString();
    return {};
  }
  QByteArray jsonData = file.readAll();
  file.close();
  QJsonDocument doc = QJsonDocument::fromJson(jsonData);
  if(doc.isNull())
  {
    qCWarning(lcDefaultsLoader) << "Failed to parse defaults.json";
  }
  return doc;
}
