#ifndef MPVDECK_VIDEOVIEWMODEL_H
#define MPVDECK_VIDEOVIEWMODEL_H

#include <QObject>
#include <QString>

class VideoViewModel : public QObject
{
  Q_OBJECT
    Q_PROPERTY(
        QString videoVo READ videoVo WRITE setVideoVo NOTIFY videoVoChanged)
    Q_PROPERTY(QString videoProfile READ videoProfile WRITE setVideoProfile
                   NOTIFY videoProfileChanged)
    Q_PROPERTY(QString videoScale READ videoScale WRITE setVideoScale NOTIFY
                   videoScaleChanged)
    Q_PROPERTY(QString videoCscale READ videoCscale WRITE setVideoCscale NOTIFY
                   videoCscaleChanged)
    Q_PROPERTY(QString videoDscale READ videoDscale WRITE setVideoDscale NOTIFY
                   videoDscaleChanged)
    Q_PROPERTY(bool videoInterpolation READ videoInterpolation WRITE
                   setVideoInterpolation NOTIFY videoInterpolationChanged)
    Q_PROPERTY(QString videoTscale READ videoTscale WRITE setVideoTscale NOTIFY
                   videoTscaleChanged)
    Q_PROPERTY(QString videoVideoSync READ videoVideoSync WRITE
                   setVideoVideoSync NOTIFY videoVideoSyncChanged)
    Q_PROPERTY(bool videoDeband READ videoDeband WRITE setVideoDeband NOTIFY
                   videoDebandChanged)

  public:
    explicit VideoViewModel(QObject* parent = nullptr);

    [[nodiscard]] auto videoVo() const -> const QString&;
    void               setVideoVo(const QString& videoVo);

    [[nodiscard]] auto videoProfile() const -> const QString&;
    void               setVideoProfile(const QString& videoProfile);

    [[nodiscard]] auto videoScale() const -> const QString&;
    void               setVideoScale(const QString& videoScale);

    [[nodiscard]] auto videoCscale() const -> const QString&;
    void               setVideoCscale(const QString& videoCscale);

    [[nodiscard]] auto videoDscale() const -> const QString&;
    void               setVideoDscale(const QString& videoDscale);

    [[nodiscard]] auto videoInterpolation() const -> bool;
    void               setVideoInterpolation(bool videoInterpolation);

    [[nodiscard]] auto videoTscale() const -> const QString&;
    void               setVideoTscale(const QString& videoTscale);

    [[nodiscard]] auto videoVideoSync() const -> const QString&;
    void               setVideoVideoSync(const QString& videoVideoSync);

    [[nodiscard]] auto videoDeband() const -> bool;
    void               setVideoDeband(bool videoDeband);

    [[nodiscard]] static auto videoProfiles() -> QStringList;
    [[nodiscard]] static auto videoScales() -> QStringList;
    [[nodiscard]] static auto videoCscales() -> QStringList;
    [[nodiscard]] static auto videoDscales() -> QStringList;
    [[nodiscard]] static auto videoTscales() -> QStringList;
    [[nodiscard]] static auto videoVoOptions() -> QStringList;
    [[nodiscard]] static auto videoVideoSyncOptions() -> QStringList;

  signals:
    void videoVoChanged(const QString& videoVo);
    void videoProfileChanged(const QString& videoProfile);
    void videoScaleChanged(const QString& videoScale);
    void videoCscaleChanged(const QString& videoCscale);
    void videoDscaleChanged(const QString& videoDscale);
    void videoInterpolationChanged(bool videoInterpolation);
    void videoTscaleChanged(const QString& videoTscale);
    void videoVideoSyncChanged(const QString& videoVideoSync);
    void videoDebandChanged(bool videoDeband);

  private:
    QString m_videoVo;
    QString m_videoProfile;
    QString m_videoScale;
    QString m_videoCscale;
    QString m_videoDscale;
    bool    m_videoInterpolation = false;
    QString m_videoTscale;
    QString m_videoVideoSync;
    bool    m_videoDeband = false;
};

#endif // MPVDECK_VIDEOVIEWMODEL_H
