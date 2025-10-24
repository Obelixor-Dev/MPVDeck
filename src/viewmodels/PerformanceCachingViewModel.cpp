#include "PerformanceCachingViewModel.h"
#include <QLoggingCategory> // New include
#include <algorithm>        // New include

Q_LOGGING_CATEGORY(
    lcPerformanceCachingViewModel,
    "mpvdeck.performancecachingviewmodel") // New logging category

PerformanceCachingViewModel::PerformanceCachingViewModel(QObject* parent)
    : QObject(parent)
{
}

auto PerformanceCachingViewModel::cache() const -> bool
{
  return m_cache;
}

void PerformanceCachingViewModel::setCache(bool cache)
{
  if(m_cache != cache)
  {
    m_cache = cache;
    emit cacheChanged(m_cache);
  }
}

auto PerformanceCachingViewModel::cacheSecs() const -> int
{
  return m_cacheSecs;
}

void PerformanceCachingViewModel::setCacheSecs(int cacheSecs)
{
  cacheSecs = std::clamp(cacheSecs, 0, MPVDeck::Constants::MAX_CACHE_SECS);
  if(m_cacheSecs != cacheSecs)
  {
    m_cacheSecs = cacheSecs;
    emit cacheSecsChanged(m_cacheSecs);
  }
}

auto PerformanceCachingViewModel::demuxerMaxBytes() const -> const QString&
{
  return m_demuxerMaxBytes;
}

void PerformanceCachingViewModel::setDemuxerMaxBytes(
    const QString& demuxerMaxBytes)
{
  if(m_demuxerMaxBytes != demuxerMaxBytes)
  {
    m_demuxerMaxBytes = demuxerMaxBytes;
    emit demuxerMaxBytesChanged(demuxerMaxBytes);
  }
}

auto PerformanceCachingViewModel::hwdec() const -> const QString&
{
  return m_hwdec;
}

void PerformanceCachingViewModel::setHwdec(const QString& hwdec)
{
  if(!hwdecOptions().contains(hwdec))
  {
    qCWarning(lcPerformanceCachingViewModel)
        << "Attempted to set invalid hardware decoding option:" << hwdec;
    return;
  }
  if(m_hwdec != hwdec)
  {
    m_hwdec = hwdec;
    emit hwdecChanged(m_hwdec);
  }
}

auto PerformanceCachingViewModel::hwdecOptions() -> QStringList
{
  return {"auto",  "auto-safe", "vaapi",        "nvdec",
          "vdpau", "dxva2",     "videotoolbox", "none"};
}

auto PerformanceCachingViewModel::hwdecCodecs() const -> const QString&
{
  return m_hwdecCodecs;
}

void PerformanceCachingViewModel::setHwdecCodecs(const QString& hwdecCodecs)
{
  if(m_hwdecCodecs != hwdecCodecs)
  {
    m_hwdecCodecs = hwdecCodecs;
    emit hwdecCodecsChanged(hwdecCodecs);
  }
}
