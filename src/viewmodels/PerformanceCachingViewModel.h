#ifndef MPVDECK_PERFORMANCECACHINGVIEWMODEL_H
#define MPVDECK_PERFORMANCECACHINGVIEWMODEL_H

#include <QObject>
#include <QString>
#include "../Constants.h"

class PerformanceCachingViewModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool cache READ cache WRITE setCache NOTIFY cacheChanged)
  Q_PROPERTY(
      int cacheSecs READ cacheSecs WRITE setCacheSecs NOTIFY cacheSecsChanged)
  Q_PROPERTY(QString demuxerMaxBytes READ demuxerMaxBytes WRITE
                 setDemuxerMaxBytes NOTIFY demuxerMaxBytesChanged)
  Q_PROPERTY(QString hwdec READ hwdec WRITE setHwdec NOTIFY hwdecChanged)
  Q_PROPERTY(QString hwdecCodecs READ hwdecCodecs WRITE setHwdecCodecs NOTIFY
                 hwdecCodecsChanged)

public:
  explicit PerformanceCachingViewModel(QObject *parent = nullptr);

  [[nodiscard]] auto cache() const -> bool;
  void setCache(bool cache);

  [[nodiscard]] auto cacheSecs() const -> int;
  void setCacheSecs(int cacheSecs);

  [[nodiscard]] auto demuxerMaxBytes() const -> QString;
  void setDemuxerMaxBytes(const QString &demuxerMaxBytes);

  [[nodiscard]] auto hwdec() const -> QString;
  void setHwdec(const QString &hwdec);

  [[nodiscard]] auto hwdecCodecs() const -> QString;
  void setHwdecCodecs(const QString &hwdecCodecs);

signals:
  void cacheChanged(bool cache);
  void cacheSecsChanged(int cacheSecs);
  void demuxerMaxBytesChanged(const QString &demuxerMaxBytes);
  void hwdecChanged(const QString &hwdec);
  void hwdecCodecsChanged(const QString &hwdecCodecs);

private:
  bool m_cache = true;
  int m_cacheSecs = MPVDeck::Constants::DEFAULT_CACHE_SECS; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  QString m_demuxerMaxBytes;
  QString m_hwdec;
  QString m_hwdecCodecs;
};

#endif // MPVDECK_PERFORMANCECACHINGVIEWMODEL_H
