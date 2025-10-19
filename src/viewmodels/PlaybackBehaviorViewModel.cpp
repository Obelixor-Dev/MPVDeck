#include "PlaybackBehaviorViewModel.h"

PlaybackBehaviorViewModel::PlaybackBehaviorViewModel(QObject *parent) : QObject(parent) {}

auto PlaybackBehaviorViewModel::resumePlayback() const -> bool { return m_resumePlayback; }

void PlaybackBehaviorViewModel::setResumePlayback(bool resumePlayback) {
    if (m_resumePlayback != resumePlayback) {
        m_resumePlayback = resumePlayback;
        emit resumePlaybackChanged(m_resumePlayback);
    }
}

auto PlaybackBehaviorViewModel::savePositionOnQuit() const -> bool { return m_savePositionOnQuit; }

void PlaybackBehaviorViewModel::setSavePositionOnQuit(bool savePositionOnQuit) {
    if (m_savePositionOnQuit != savePositionOnQuit) {
        m_savePositionOnQuit = savePositionOnQuit;
        emit savePositionOnQuitChanged(m_savePositionOnQuit);
    }
}

auto PlaybackBehaviorViewModel::loopFile() const -> QString { return m_loopFile; }

void PlaybackBehaviorViewModel::setLoopFile(const QString &loopFile) {
    if (m_loopFile != loopFile) {
        m_loopFile = loopFile;
        emit loopFileChanged(loopFile);
    }
}

auto PlaybackBehaviorViewModel::keepOpen() const -> QString { return m_keepOpen; }

void PlaybackBehaviorViewModel::setKeepOpen(const QString &keepOpen) {
    if (m_keepOpen != keepOpen) {
        m_keepOpen = keepOpen;
        emit keepOpenChanged(keepOpen);
    }
}

auto PlaybackBehaviorViewModel::autofitLarger() const -> QString { return m_autofitLarger; }

void PlaybackBehaviorViewModel::setAutofitLarger(const QString &autofitLarger) {
    if (m_autofitLarger != autofitLarger) {
        m_autofitLarger = autofitLarger;
        emit autofitLargerChanged(autofitLarger);
    }
}

auto PlaybackBehaviorViewModel::ytdlRawOptions() const -> QString { return m_ytdlRawOptions; }

void PlaybackBehaviorViewModel::setYtdlRawOptions(const QString &ytdlRawOptions) {
    if (m_ytdlRawOptions != ytdlRawOptions) {
        m_ytdlRawOptions = ytdlRawOptions;
        emit ytdlRawOptionsChanged(ytdlRawOptions);
    }
}
