#include "SubtitleViewModel.h"

SubtitleViewModel::SubtitleViewModel(QObject *parent) : QObject(parent) {}

auto SubtitleViewModel::subtitleVisibility() const -> bool {
  return m_subtitleVisibility;
}

void SubtitleViewModel::setSubtitleVisibility(bool subtitleVisibility) {
  if (m_subtitleVisibility != subtitleVisibility) {
    m_subtitleVisibility = subtitleVisibility;
    emit subtitleVisibilityChanged(m_subtitleVisibility);
  }
}

auto SubtitleViewModel::subtitleFontSize() const -> int {
  return m_subtitleFontSize;
}

void SubtitleViewModel::setSubtitleFontSize(int subtitleFontSize) {
  if (m_subtitleFontSize != subtitleFontSize) {
    m_subtitleFontSize = subtitleFontSize;
    emit subtitleFontSizeChanged(m_subtitleFontSize);
  }
}

auto SubtitleViewModel::subtitleColor() const -> QString {
  return m_subtitleColor;
}

void SubtitleViewModel::setSubtitleColor(const QString &subtitleColor) {
  if (m_subtitleColor != subtitleColor) {
    m_subtitleColor = subtitleColor;
    emit subtitleColorChanged(m_subtitleColor);
  }
}

auto SubtitleViewModel::subtitleLanguages() const -> QString {
  return m_subtitleLanguages;
}

void SubtitleViewModel::setSubtitleLanguages(const QString &subtitleLanguages) {
  if (m_subtitleLanguages != subtitleLanguages) {
    m_subtitleLanguages = subtitleLanguages;
    emit subtitleLanguagesChanged(m_subtitleLanguages);
  }
}

auto SubtitleViewModel::subAuto() const -> QString { return m_subAuto; }

void SubtitleViewModel::setSubAuto(const QString &subAuto) {
  if (m_subAuto != subAuto) {
    m_subAuto = subAuto;
    emit subAutoChanged(subAuto);
  }
}

auto SubtitleViewModel::sid() const -> QString { return m_sid; }

void SubtitleViewModel::setSid(const QString &sid) {
  if (m_sid != sid) {
    m_sid = sid;
    emit sidChanged(m_sid);
  }
}

auto SubtitleViewModel::subForcedOnly() const -> bool {
  return m_subForcedOnly;
}

void SubtitleViewModel::setSubForcedOnly(bool subForcedOnly) {
  if (m_subForcedOnly != subForcedOnly) {
    m_subForcedOnly = subForcedOnly;
    emit subForcedOnlyChanged(subForcedOnly);
  }
}

auto SubtitleViewModel::subFont() const -> QString { return m_subFont; }

void SubtitleViewModel::setSubFont(const QString &subFont) {
  if (m_subFont != subFont) {
    m_subFont = subFont;
    emit subFontChanged(m_subFont);
  }
}
