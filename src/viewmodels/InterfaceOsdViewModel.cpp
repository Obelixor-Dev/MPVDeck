#include "InterfaceOsdViewModel.h"

InterfaceOsdViewModel::InterfaceOsdViewModel(QObject *parent) : QObject(parent) {}

auto InterfaceOsdViewModel::osdLevel() const -> int { return m_osdLevel; }

void InterfaceOsdViewModel::setOsdLevel(int osdLevel) {
    if (osdLevel < 0) osdLevel = 0;
    if (osdLevel > 3) osdLevel = 3;
    if (m_osdLevel != osdLevel) {
        m_osdLevel = osdLevel;
        emit osdLevelChanged(m_osdLevel);
    }
}

auto InterfaceOsdViewModel::osdFontSize() const -> int { return m_osdFontSize; }

void InterfaceOsdViewModel::setOsdFontSize(int osdFontSize) {
    if (osdFontSize < 10) osdFontSize = 10;
    if (osdFontSize > 100) osdFontSize = 100;
    if (m_osdFontSize != osdFontSize) {
        m_osdFontSize = osdFontSize;
        emit osdFontSizeChanged(m_osdFontSize);
    }
}

auto InterfaceOsdViewModel::osdDuration() const -> int { return m_osdDuration; }

void InterfaceOsdViewModel::setOsdDuration(int osdDuration) {
    if (m_osdDuration != osdDuration) {
        m_osdDuration = osdDuration;
        emit osdDurationChanged(m_osdDuration);
    }
}

auto InterfaceOsdViewModel::osc() const -> bool { return m_osc; }

void InterfaceOsdViewModel::setOsc(bool osc) {
    if (m_osc != osc) {
        m_osc = osc;
        emit oscChanged(m_osc);
    }
}

auto InterfaceOsdViewModel::screenshotFormat() const -> QString { return m_screenshotFormat; }

void InterfaceOsdViewModel::setScreenshotFormat(const QString &screenshotFormat) {
    if (m_screenshotFormat != screenshotFormat) {
        m_screenshotFormat = screenshotFormat;
        emit screenshotFormatChanged(m_screenshotFormat);
    }
}

auto InterfaceOsdViewModel::screenshotDirectory() const -> QString { return m_screenshotDirectory; }

void InterfaceOsdViewModel::setScreenshotDirectory(const QString &screenshotDirectory) {
    if (m_screenshotDirectory != screenshotDirectory) {
        m_screenshotDirectory = screenshotDirectory;
        emit screenshotDirectoryChanged(m_screenshotDirectory);
    }
}

auto InterfaceOsdViewModel::screenshotTemplate() const -> QString { return m_screenshotTemplate; }

void InterfaceOsdViewModel::setScreenshotTemplate(const QString &screenshotTemplate) {
    if (m_screenshotTemplate != screenshotTemplate) {
        m_screenshotTemplate = screenshotTemplate;
        emit screenshotTemplateChanged(m_screenshotTemplate);
    }
}
