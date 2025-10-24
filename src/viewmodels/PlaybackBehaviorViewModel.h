#ifndef MPVDECK_PLAYBACKBEHAVIORVIEWMODEL_H
#define MPVDECK_PLAYBACKBEHAVIORVIEWMODEL_H

#include <QObject>
#include <QString>

class PlaybackBehaviorViewModel : public QObject
{
  Q_OBJECT
    Q_PROPERTY(bool resumePlayback READ resumePlayback WRITE setResumePlayback
                   NOTIFY resumePlaybackChanged)
    Q_PROPERTY(bool savePositionOnQuit READ savePositionOnQuit WRITE
                   setSavePositionOnQuit NOTIFY savePositionOnQuitChanged)
    Q_PROPERTY(
        QString loopFile READ loopFile WRITE setLoopFile NOTIFY loopFileChanged)
    Q_PROPERTY(
        QString keepOpen READ keepOpen WRITE setKeepOpen NOTIFY keepOpenChanged)
    Q_PROPERTY(QString autofitLarger READ autofitLarger WRITE setAutofitLarger
                   NOTIFY autofitLargerChanged)
    Q_PROPERTY(QString ytdlRawOptions READ ytdlRawOptions WRITE
                   setYtdlRawOptions NOTIFY ytdlRawOptionsChanged)

  public:
    explicit PlaybackBehaviorViewModel(QObject* parent = nullptr);

    [[nodiscard]] auto resumePlayback() const -> bool;
    void               setResumePlayback(bool resumePlayback);

    [[nodiscard]] auto savePositionOnQuit() const -> bool;
    void               setSavePositionOnQuit(bool savePositionOnQuit);

    [[nodiscard]] auto loopFile() const -> const QString&;
    void               setLoopFile(const QString& loopFile);

    [[nodiscard]] static auto loopFileOptions() -> QStringList;

    [[nodiscard]] auto keepOpen() const -> const QString&;
    void               setKeepOpen(const QString& keepOpen);

    [[nodiscard]] static auto keepOpenOptions() -> QStringList;

    [[nodiscard]] auto autofitLarger() const -> const QString&;
    void               setAutofitLarger(const QString& autofitLarger);

    [[nodiscard]] auto ytdlRawOptions() const -> const QString&;
    void               setYtdlRawOptions(const QString& ytdlRawOptions);

  signals:
    void resumePlaybackChanged(bool resumePlayback);
    void savePositionOnQuitChanged(bool savePositionOnQuit);
    void loopFileChanged(const QString& loopFile);
    void keepOpenChanged(const QString& keepOpen);
    void autofitLargerChanged(const QString& autofitLarger);
    void ytdlRawOptionsChanged(const QString& ytdlRawOptions);

  private:
    bool    m_resumePlayback     = false;
    bool    m_savePositionOnQuit = false;
    QString m_loopFile;
    QString m_keepOpen;
    QString m_autofitLarger;
    QString m_ytdlRawOptions;
};

#endif // MPVDECK_PLAYBACKBEHAVIORVIEWMODEL_H
