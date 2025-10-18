#ifndef MPVDECK_SETTINGSVIEWMODEL_H
#define MPVDECK_SETTINGSVIEWMODEL_H

#include <QObject>
#include <QMap>
#include <QString>

class SettingsViewModel : public QObject {
    Q_OBJECT
public:
    explicit SettingsViewModel(QObject *parent = nullptr);

    void loadSettings();

    QMap<QString, QString> settings() const;

private:
    QMap<QString, QString> m_settings;
};

#endif //MPVDECK_SETTINGSVIEWMODEL_H
