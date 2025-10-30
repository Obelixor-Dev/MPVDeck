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

// ... (other methods)

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
    emit videoVideoSyncChanged(m_videoVideoSync);
  }
}

// ... (other methods)

auto VideoViewModel::videoVoOptions() -> QStringList
{
  return {"gpu", "gpu-next", "x11", "xv", "opengl", "sdl"};
}

auto VideoViewModel::videoVideoSyncOptions() -> QStringList
{
  return {"display-resample", "audio", "video"};
}
