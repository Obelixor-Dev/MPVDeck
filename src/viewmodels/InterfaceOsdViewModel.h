#ifndef MPVDECK_INTERFACEOSDVIEWMODEL_H
#define MPVDECK_INTERFACEOSDVIEWMODEL_H

#include <QObject>
#include <QString>

class InterfaceOsdViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(int osdLevel READ osdLevel WRITE setOsdLevel NOTIFY osdLevelChanged)
    Q_PROPERTY(int osdFontSize READ osdFontSize WRITE setOsdFontSize NOTIFY osdFontSizeChanged)
    Q_PROPERTY(int osdDuration READ osdDuration WRITE setOsdDuration NOTIFY osdDurationChanged)
    Q_PROPERTY(bool osc READ osc WRITE setOsc NOTIFY oscChanged)
    Q_PROPERTY(QString screenshotFormat READ screenshotFormat WRITE setScreenshotFormat NOTIFY screenshotFormatChanged)
    Q_PROPERTY(QString screenshotDirectory READ screenshotDirectory WRITE setScreenshotDirectory NOTIFY screenshotDirectoryChanged)
    Q_PROPERTY(QString screenshotTemplate READ screenshotTemplate WRITE setScreenshotTemplate NOTIFY screenshotTemplateChanged)

public:
    explicit InterfaceOsdViewModel(QObject *parent = nullptr);

    [[nodiscard]] auto osdLevel() const -> int;
    void setOsdLevel(int osdLevel);

    [[nodiscard]] auto osdFontSize() const -> int;
    void setOsdFontSize(int osdFontSize);

    [[nodiscard]] auto osdDuration() const -> int;
    void setOsdDuration(int osdDuration);

    [[nodiscard]] auto osc() const -> bool;
    void setOsc(bool osc);

    [[nodiscard]] auto screenshotFormat() const -> QString;
    void setScreenshotFormat(const QString &screenshotFormat);

    [[nodiscard]] auto screenshotDirectory() const -> QString;
    void setScreenshotDirectory(const QString &screenshotDirectory);

    [[nodiscard]] auto screenshotTemplate() const -> QString;
    void setScreenshotTemplate(const QString &screenshotTemplate);

signals:
    void osdLevelChanged(int osdLevel);
    void osdFontSizeChanged(int osdFontSize);
    void osdDurationChanged(int osdDuration);
    void oscChanged(bool osc);
    void screenshotFormatChanged(const QString &screenshotFormat);
    void screenshotDirectoryChanged(const QString &screenshotDirectory);
    void screenshotTemplateChanged(const QString &screenshotTemplate);

private:
    int m_osdLevel = 1;
    int m_osdFontSize = 28;
    int m_osdDuration = 2000;
    bool m_osc = true;
    QString m_screenshotFormat;
    QString m_screenshotDirectory;
    QString m_screenshotTemplate;
};

#endif // MPVDECK_INTERFACEOSDVIEWMODEL_H
