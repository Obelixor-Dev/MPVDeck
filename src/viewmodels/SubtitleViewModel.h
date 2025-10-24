#ifndef MPVDECK_SUBTITLEVIEWMODEL_H
#define MPVDECK_SUBTITLEVIEWMODEL_H

#include <QObject>
#include <QString>

const int DEFAULT_SUBTITLE_FONT_SIZE = 28;

class SubtitleViewModel : public QObject
{
  Q_OBJECT
    Q_PROPERTY(bool subtitleVisibility READ subtitleVisibility WRITE
                   setSubtitleVisibility NOTIFY subtitleVisibilityChanged)
    Q_PROPERTY(int subtitleFontSize READ subtitleFontSize WRITE
                   setSubtitleFontSize NOTIFY subtitleFontSizeChanged)
    Q_PROPERTY(QString subtitleColor READ subtitleColor WRITE setSubtitleColor
                   NOTIFY subtitleColorChanged)
    Q_PROPERTY(QString subtitleLanguages READ subtitleLanguages WRITE
                   setSubtitleLanguages NOTIFY subtitleLanguagesChanged)
    Q_PROPERTY(
        QString subAuto READ subAuto WRITE setSubAuto NOTIFY subAutoChanged)
    Q_PROPERTY(QString sid READ sid WRITE setSid NOTIFY sidChanged)
    Q_PROPERTY(bool subForcedOnly READ subForcedOnly WRITE setSubForcedOnly
                   NOTIFY subForcedOnlyChanged)
    Q_PROPERTY(
        QString subFont READ subFont WRITE setSubFont NOTIFY subFontChanged)

  public:
    explicit SubtitleViewModel(QObject* parent = nullptr);

    [[nodiscard]] auto subtitleVisibility() const -> bool;
    void               setSubtitleVisibility(bool subtitleVisibility);

    [[nodiscard]] auto subtitleFontSize() const -> int;
    void               setSubtitleFontSize(int subtitleFontSize);

    [[nodiscard]] auto subtitleColor() const -> const QString&;
    void               setSubtitleColor(const QString& subtitleColor);

    [[nodiscard]] auto subtitleLanguages() const -> const QString&;
    void               setSubtitleLanguages(const QString& subtitleLanguages);

    [[nodiscard]] auto subAuto() const -> const QString&;
    void               setSubAuto(const QString& subAuto);

    [[nodiscard]] static auto subAutoOptions() -> QStringList;

    [[nodiscard]] auto sid() const -> const QString&;
    void               setSid(const QString& sid);

    [[nodiscard]] auto subForcedOnly() const -> bool;
    void               setSubForcedOnly(bool subForcedOnly);

    [[nodiscard]] auto subFont() const -> const QString&;
    void               setSubFont(const QString& subFont);

  signals:
    void subtitleVisibilityChanged(bool subtitleVisibility);
    void subtitleFontSizeChanged(int subtitleFontSize);
    void subtitleColorChanged(const QString& subtitleColor);
    void subtitleLanguagesChanged(const QString& subtitleLanguages);
    void subAutoChanged(const QString& subAuto);
    void sidChanged(const QString& sid);
    void subForcedOnlyChanged(bool subForcedOnly);
    void subFontChanged(const QString& subFont);
    void errorOccurred(const QString& message);

  private:
    bool    m_subtitleVisibility = true;
    int     m_subtitleFontSize   = DEFAULT_SUBTITLE_FONT_SIZE;
    QString m_subtitleColor;
    QString m_subtitleLanguages;
    QString m_subAuto;
    QString m_sid;
    bool    m_subForcedOnly = false;
    QString m_subFont;
};

#endif // MPVDECK_SUBTITLEVIEWMODEL_H
