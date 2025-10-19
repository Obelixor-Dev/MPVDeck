#include "PerformanceCachingViewModel.h"

PerformanceCachingViewModel::PerformanceCachingViewModel(QObject *parent) : QObject(parent) {}

auto PerformanceCachingViewModel::cache() const -> bool { return m_cache; }

void PerformanceCachingViewModel::setCache(bool cache) {
    if (m_cache != cache) {
        m_cache = cache;
        emit cacheChanged(m_cache);
    }
}

auto PerformanceCachingViewModel::cacheSecs() const -> int { return m_cacheSecs; }

void PerformanceCachingViewModel::setCacheSecs(int cacheSecs) {
    if (m_cacheSecs != cacheSecs) {
        m_cacheSecs = cacheSecs;
        emit cacheSecsChanged(m_cacheSecs);
    }
}

auto PerformanceCachingViewModel::demuxerMaxBytes() const -> QString { return m_demuxerMaxBytes; }

void PerformanceCachingViewModel::setDemuxerMaxBytes(const QString &demuxerMaxBytes) {
    if (m_demuxerMaxBytes != demuxerMaxBytes) {
        m_demuxerMaxBytes = demuxerMaxBytes;
        emit demuxerMaxBytesChanged(demuxerMaxBytes);
    }
}

auto PerformanceCachingViewModel::hwdec() const -> QString { return m_hwdec; }

void PerformanceCachingViewModel::setHwdec(const QString &hwdec) {
    if (m_hwdec != hwdec) {
        m_hwdec = hwdec;
        emit hwdecChanged(m_hwdec);
    }
}

auto PerformanceCachingViewModel::hwdecCodecs() const -> QString { return m_hwdecCodecs; }

void PerformanceCachingViewModel::setHwdecCodecs(const QString &hwdecCodecs) {
    if (m_hwdecCodecs != hwdecCodecs) {
        m_hwdecCodecs = hwdecCodecs;
        emit hwdecCodecsChanged(hwdecCodecs);
    }
}
