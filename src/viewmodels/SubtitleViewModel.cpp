#include "SubtitleViewModel.h"
#include "../../src/utils/ColorUtils.h"
#include <QColor>
#include <QLoggingCategory>
#include <algorithm>

Q_LOGGING_CATEGORY(lcSubtitleViewModel,
                   "mpvdeck.subtitleviewmodel") // New logging category

SubtitleViewModel::SubtitleViewModel(QObject* parent) : QObject(parent) {}

auto SubtitleViewModel::subtitleVisibility() const -> bool
{
  return m_subtitleVisibility;
}

void SubtitleViewModel::setSubtitleVisibility(bool subtitleVisibility)
{
  if(m_subtitleVisibility != subtitleVisibility)
  {
    m_subtitleVisibility = subtitleVisibility;
    emit subtitleVisibilityChanged(m_subtitleVisibility);
  }
}

auto SubtitleViewModel::subtitleFontSize() const -> int
{
  return m_subtitleFontSize;
}

void SubtitleViewModel::setSubtitleFontSize(int subtitleFontSize)
{
  subtitleFontSize = std::clamp(subtitleFontSize, 10, 100);
  if(m_subtitleFontSize != subtitleFontSize)
  {
    m_subtitleFontSize = subtitleFontSize;
    emit subtitleFontSizeChanged(m_subtitleFontSize);
  }
}

auto SubtitleViewModel::subtitleColor() const -> const QString&
{
  return m_subtitleColor;
}

void SubtitleViewModel::setSubtitleColor(const QString& subtitleColor)
{
  QString normalizedColor = ColorUtils::normalizeColor(subtitleColor);
  if(normalizedColor.isEmpty())
  {
    emit errorOccurred(
        "Invalid color format. Please use a valid hex, named, or rgb() color.");
    return;
  }
  if(m_subtitleColor != normalizedColor)
  {
    m_subtitleColor = normalizedColor;
    emit subtitleColorChanged(m_subtitleColor);
  }
}

auto SubtitleViewModel::subtitleLanguages() const -> const QString&
{
  return m_subtitleLanguages;
}

void SubtitleViewModel::setSubtitleLanguages(const QString& subtitleLanguages)
{
  if(m_subtitleLanguages != subtitleLanguages)
  {
    m_subtitleLanguages = subtitleLanguages;
    emit subtitleLanguagesChanged(m_subtitleLanguages);
  }
}

auto SubtitleViewModel::subAuto() const -> const QString&
{
  return m_subAuto;
}

void SubtitleViewModel::setSubAuto(const QString& subAuto)
{
  if(!subAutoOptions().contains(subAuto))
  {
    qCWarning(lcSubtitleViewModel)
        << "Attempted to set invalid sub auto option:" << subAuto;
    return;
  }
  if(m_subAuto != subAuto)
  {
    m_subAuto = subAuto;
    emit subAutoChanged(subAuto);
  }
}

auto SubtitleViewModel::subAutoOptions() -> QStringList
{
  return {"no", "fuzzy", "exact"};
}

auto SubtitleViewModel::sid() const -> const QString&
{
  return m_sid;
}

void SubtitleViewModel::setSid(const QString& sid)
{
  if(m_sid != sid)
  {
    m_sid = sid;
    emit sidChanged(m_sid);
  }
}

auto SubtitleViewModel::subForcedOnly() const -> bool
{
  return m_subForcedOnly;
}

void SubtitleViewModel::setSubForcedOnly(bool subForcedOnly)
{
  if(m_subForcedOnly != subForcedOnly)
  {
    m_subForcedOnly = subForcedOnly;
    emit subForcedOnlyChanged(subForcedOnly);
  }
}

auto SubtitleViewModel::subFont() const -> const QString&
{
  return m_subFont;
}

void SubtitleViewModel::setSubFont(const QString& subFont)
{
  if(m_subFont != subFont)
  {
    m_subFont = subFont;
    emit subFontChanged(m_subFont);
  }
}
