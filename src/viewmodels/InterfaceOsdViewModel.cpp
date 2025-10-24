#include "InterfaceOsdViewModel.h"
#include "Constants.h"
#include <QDebug>
#include <QLoggingCategory> // New include
#include <algorithm>

Q_LOGGING_CATEGORY(lcInterfaceOsdViewModel,
                   "mpvdeck.interfaceosdviewmodel") // New logging category

using namespace MPVDeck;
using namespace MPVDeck::Constants;

InterfaceOsdViewModel::InterfaceOsdViewModel(QObject* parent) : QObject(parent)
{
}

auto InterfaceOsdViewModel::osdLevel() const -> int
{
  return m_osdLevel;
}

void InterfaceOsdViewModel::setOsdLevel(int osdLevel)
{
  osdLevel = std::clamp(osdLevel, MIN_OSD_LEVEL, MAX_OSD_LEVEL);
  if(m_osdLevel != osdLevel)
  {
    m_osdLevel = osdLevel;
    emit osdLevelChanged(m_osdLevel);
  }
}

auto InterfaceOsdViewModel::osdFontSize() const -> int
{
  return m_osdFontSize;
}

void InterfaceOsdViewModel::setOsdFontSize(int osdFontSize)
{
  osdFontSize = std::clamp(osdFontSize, MIN_OSD_FONT_SIZE, MAX_OSD_FONT_SIZE);
  if(m_osdFontSize != osdFontSize)
  {
    m_osdFontSize = osdFontSize;
    emit osdFontSizeChanged(m_osdFontSize);
  }
}

auto InterfaceOsdViewModel::osdDuration() const -> int
{
  return m_osdDuration;
}

void InterfaceOsdViewModel::setOsdDuration(int osdDuration)
{
  osdDuration = std::clamp(osdDuration, MIN_OSD_DURATION, MAX_OSD_DURATION);
  if(m_osdDuration != osdDuration)
  {
    m_osdDuration = osdDuration;
    emit osdDurationChanged(m_osdDuration);
  }
}

auto InterfaceOsdViewModel::osc() const -> bool
{
  return m_osc;
}

void InterfaceOsdViewModel::setOsc(bool osc)
{
  if(m_osc != osc)
  {
    m_osc = osc;
    emit oscChanged(m_osc);
  }
}

auto InterfaceOsdViewModel::screenshotFormat() const -> const QString&
{
  return m_screenshotFormat;
}

void InterfaceOsdViewModel::setScreenshotFormat(const QString& screenshotFormat)
{
  if(!screenshotFormatOptions().contains(screenshotFormat))
  {
    qCWarning(lcInterfaceOsdViewModel)
        << "Attempted to set invalid screenshot format:" << screenshotFormat;
    return;
  }
  if(m_screenshotFormat != screenshotFormat)
  {
    m_screenshotFormat = screenshotFormat;
    emit screenshotFormatChanged(m_screenshotFormat);
  }
}

auto InterfaceOsdViewModel::screenshotFormatOptions() -> QStringList
{
  return {"png", "jpg", "bmp"};
}

auto InterfaceOsdViewModel::screenshotDirectory() const -> const QString&
{
  return m_screenshotDirectory;
}

void InterfaceOsdViewModel::setScreenshotDirectory(
    const QString& screenshotDirectory)
{
  if(m_screenshotDirectory != screenshotDirectory)
  {
    m_screenshotDirectory = screenshotDirectory;
    emit screenshotDirectoryChanged(m_screenshotDirectory);
  }
}

auto InterfaceOsdViewModel::screenshotTemplate() const -> const QString&
{
  return m_screenshotTemplate;
}

void InterfaceOsdViewModel::setScreenshotTemplate(
    const QString& screenshotTemplate)
{
  if(m_screenshotTemplate != screenshotTemplate)
  {
    m_screenshotTemplate = screenshotTemplate;
    emit screenshotTemplateChanged(m_screenshotTemplate);
  }
}

auto InterfaceOsdViewModel::videoSyncOptions() -> QStringList
{
  return {"display-resample", "audio", "video"};
}
