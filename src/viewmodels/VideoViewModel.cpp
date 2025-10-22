#include "VideoViewModel.h"

VideoViewModel::VideoViewModel(QObject *parent) : QObject(parent) {}

auto VideoViewModel::videoVo() const -> QString { return m_videoVo; }

void VideoViewModel::setVideoVo(const QString &videoVo) {
  if (m_videoVo != videoVo) {
    m_videoVo = videoVo;
    emit videoVoChanged(m_videoVo);
  }
}

auto VideoViewModel::videoProfile() const -> QString { return m_videoProfile; }

void VideoViewModel::setVideoProfile(const QString &videoProfile) {
  if (m_videoProfile != videoProfile) {
    m_videoProfile = videoProfile;
    emit videoProfileChanged(m_videoProfile);
  }
}

auto VideoViewModel::videoScale() const -> QString { return m_videoScale; }

void VideoViewModel::setVideoScale(const QString &videoScale) {
  if (m_videoScale != videoScale) {
    m_videoScale = videoScale;
    emit videoScaleChanged(m_videoScale);
  }
}

auto VideoViewModel::videoCscale() const -> QString { return m_videoCscale; }

void VideoViewModel::setVideoCscale(const QString &videoCscale) {
  if (m_videoCscale != videoCscale) {
    m_videoCscale = videoCscale;
    emit videoCscaleChanged(m_videoCscale);
  }
}

auto VideoViewModel::videoDscale() const -> QString { return m_videoDscale; }

void VideoViewModel::setVideoDscale(const QString &videoDscale) {
  if (m_videoDscale != videoDscale) {
    m_videoDscale = videoDscale;
    emit videoDscaleChanged(m_videoDscale);
  }
}

auto VideoViewModel::videoInterpolation() const -> bool {
  return m_videoInterpolation;
}

void VideoViewModel::setVideoInterpolation(bool videoInterpolation) {
  if (m_videoInterpolation != videoInterpolation) {
    m_videoInterpolation = videoInterpolation;
    emit videoInterpolationChanged(m_videoInterpolation);
  }
}

auto VideoViewModel::videoTscale() const -> QString { return m_videoTscale; }

void VideoViewModel::setVideoTscale(const QString &videoTscale) {
  if (m_videoTscale != videoTscale) {
    m_videoTscale = videoTscale;
    emit videoTscaleChanged(m_videoTscale);
  }
}

auto VideoViewModel::videoVideoSync() const -> QString {
  return m_videoVideoSync;
}

void VideoViewModel::setVideoVideoSync(const QString &videoVideoSync) {
  if (m_videoVideoSync != videoVideoSync) {
    m_videoVideoSync = videoVideoSync;
    emit videoVideoSyncChanged(videoVideoSync);
  }
}

auto VideoViewModel::videoDeband() const -> bool { return m_videoDeband; }

void VideoViewModel::setVideoDeband(bool videoDeband) {
  if (m_videoDeband != videoDeband) {
    m_videoDeband = videoDeband;
    emit videoDebandChanged(m_videoDeband);
  }
}
