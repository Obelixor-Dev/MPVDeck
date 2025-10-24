#ifndef MPVDECK_AUDIOVIEWMODEL_H
#define MPVDECK_AUDIOVIEWMODEL_H

#include <QObject>
#include <QString>

const int DEFAULT_VOLUME = 50;

class AudioViewModel : public QObject
{
  Q_OBJECT
    Q_PROPERTY(bool mute READ mute WRITE setMute NOTIFY muteChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(QString audioDevice READ audioDevice WRITE setAudioDevice NOTIFY
                   audioDeviceChanged)
    Q_PROPERTY(QString audioChannels READ audioChannels WRITE setAudioChannels
                   NOTIFY audioChannelsChanged)
    Q_PROPERTY(double audioDelay READ audioDelay WRITE setAudioDelay NOTIFY
                   audioDelayChanged)
    Q_PROPERTY(bool audioNormalizeDownmix READ audioNormalizeDownmix WRITE
                   setAudioNormalizeDownmix NOTIFY audioNormalizeDownmixChanged)

  public:
    explicit AudioViewModel(QObject* parent = nullptr);

    [[nodiscard]] auto mute() const -> bool;
    void               setMute(bool mute);

    [[nodiscard]] auto volume() const -> int;
    void               setVolume(int volume);

    [[nodiscard]] auto audioDevice() const -> const QString&;
    void               setAudioDevice(const QString& audioDevice);

    [[nodiscard]] auto audioChannels() const -> const QString&;
    void               setAudioChannels(const QString& audioChannels);

    [[nodiscard]] static auto audioDevices() -> QStringList;
    [[nodiscard]] static auto audioChannelLayouts() -> QStringList;

    [[nodiscard]] auto audioDelay() const -> double;
    void               setAudioDelay(double audioDelay);

    [[nodiscard]] auto audioNormalizeDownmix() const -> bool;
    void               setAudioNormalizeDownmix(bool audioNormalizeDownmix);

  signals:
    void muteChanged(bool mute);
    void volumeChanged(int volume);
    void audioDeviceChanged(const QString& audioDevice);
    void audioChannelsChanged(const QString& audioChannels);
    void audioDelayChanged(double audioDelay);
    void audioNormalizeDownmixChanged(bool audioNormalizeDownmix);

  private:
    bool    m_mute   = false;
    int     m_volume = DEFAULT_VOLUME;
    QString m_audioDevice;
    QString m_audioChannels;
    double  m_audioDelay            = 0.0;
    bool    m_audioNormalizeDownmix = true;
};

#endif // MPVDECK_AUDIOVIEWMODEL_H
