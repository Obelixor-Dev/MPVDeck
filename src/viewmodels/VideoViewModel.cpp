#include "VideoViewModel.h"
#include <QLoggingCategory> // New include

Q_LOGGING_CATEGORY(lcVideoViewModel,
                   "mpvdeck.videoviewmodel") // New logging category

VideoViewModel::VideoViewModel(QObject* parent) : QObject(parent) {}

auto VideoViewModel::videoVo() const -> const QString&
{
  return m_videoVo;
}

void VideoViewModel::setVideoVo(const QString& videoVo)
{
  if(!videoVoOptions().contains(videoVo))
  {
    qCWarning(lcVideoViewModel)
        << "Attempted to set invalid video output:" << videoVo;
    return;
  }
  if(m_videoVo != videoVo)
  {
    m_videoVo = videoVo;
    emit videoVoChanged(m_videoVo);
  }
}

auto VideoViewModel::videoProfile() const -> const QString&
{
  return m_videoProfile;
}

void VideoViewModel::setVideoProfile(const QString& videoProfile)
{
  if(!videoProfiles().contains(videoProfile))
  {
    qCWarning(lcVideoViewModel)
        << "Attempted to set invalid video profile:" << videoProfile;
    return;
  }
  if(m_videoProfile != videoProfile)
  {
    m_videoProfile = videoProfile;
    emit videoProfileChanged(m_videoProfile);
  }
}

auto VideoViewModel::videoScale() const -> const QString&
{
  return m_videoScale;
}

void VideoViewModel::setVideoScale(const QString& videoScale)
{
  if(!videoScales().contains(videoScale))
  {
    qCWarning(lcVideoViewModel)
        << "Attempted to set invalid video scale:" << videoScale;
    return;
  }
  if(m_videoScale != videoScale)
  {
    m_videoScale = videoScale;
    emit videoScaleChanged(m_videoScale);
  }
}

auto VideoViewModel::videoCscale() const -> const QString&
{
  return m_videoCscale;
}

void VideoViewModel::setVideoCscale(const QString& videoCscale)
{
  if(!videoCscales().contains(videoCscale))
  {
    qCWarning(lcVideoViewModel)
        << "Attempted to set invalid video cscale:" << videoCscale;
    return;
  }
  if(m_videoCscale != videoCscale)
  {
    m_videoCscale = videoCscale;
    emit videoCscaleChanged(m_videoCscale);
  }
}

auto VideoViewModel::videoDscale() const -> const QString&
{
  return m_videoDscale;
}

void VideoViewModel::setVideoDscale(const QString& videoDscale)
{
  if(!videoDscales().contains(videoDscale))
  {
    qCWarning(lcVideoViewModel)
        << "Attempted to set invalid video dscale:" << videoDscale;
    return;
  }
  if(m_videoDscale != videoDscale)
  {
    m_videoDscale = videoDscale;
    emit videoDscaleChanged(m_videoDscale);
  }
}

auto VideoViewModel::videoInterpolation() const -> bool
{
  return m_videoInterpolation;
}

void VideoViewModel::setVideoInterpolation(bool videoInterpolation)
{
  if(m_videoInterpolation != videoInterpolation)
  {
    m_videoInterpolation = videoInterpolation;
    emit videoInterpolationChanged(m_videoInterpolation);
  }
}

auto VideoViewModel::videoTscale() const -> const QString&
{
  return m_videoTscale;
}

void VideoViewModel::setVideoTscale(const QString& videoTscale)
{
  if(!videoTscales().contains(videoTscale))
  {
    qCWarning(lcVideoViewModel)
        << "Attempted to set invalid video tscale:" << videoTscale;
    return;
  }
  if(m_videoTscale != videoTscale)
  {
    m_videoTscale = videoTscale;
    emit videoTscaleChanged(m_videoTscale);
  }
}

auto VideoViewModel::videoVideoSync() const -> const QString&
{
  return m_videoVideoSync;
}

void VideoViewModel::setVideoVideoSync(const QString& videoVideoSync)
{
  if(!videoVideoSyncOptions().contains(videoVideoSync))
  {
    qCWarning(lcVideoViewModel)
        << "Attempted to set invalid video sync:" << videoVideoSync;
    return;
  }
  if(m_videoVideoSync != videoVideoSync)
  {
    m_videoVideoSync = videoVideoSync;
    emit videoVideoSyncChanged(videoVideoSync);
  }
}

auto VideoViewModel::videoDeband() const -> bool
{
  return m_videoDeband;
}

void VideoViewModel::setVideoDeband(bool videoDeband)
{
  if(m_videoDeband != videoDeband)
  {
    m_videoDeband = videoDeband;
    emit videoDebandChanged(m_videoDeband);
  }
}

auto VideoViewModel::videoProfiles() -> QStringList
{
  return {"gpu-hq", "gpu", "low-latency", "lavfi-complex", "default"};
}

auto VideoViewModel::videoScales() -> QStringList
{
  return {"bilinear", "bicubic", "lanczos", "ewa_lanczossharp", "spline36"};
}

auto VideoViewModel::videoCscales() -> QStringList
{
  return {"bilinear", "bicubic", "lanczos", "ewa_lanczossharp", "spline36"};
}

auto VideoViewModel::videoDscales() -> QStringList
{
  return {"bilinear", "bicubic", "lanczos", "mitchell"};
}

auto VideoViewModel::videoTscales() -> QStringList
{
  return {"oversample", "linear", "nearest"};
}

auto VideoViewModel::videoVoOptions() -> QStringList
{
  return {"gpu", "gpu-next", "x11", "xv", "opengl", "sdl"};
}

auto VideoViewModel::videoVideoSyncOptions() -> QStringList
{
  return {"display-resample", "audio", "video"};
}