#ifndef MPVDECK_MPVCONFHIGHLIGHTER_H
#define MPVDECK_MPVCONFHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QtCore/QRegularExpression>

class MpvConfHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT

  public:
    explicit MpvConfHighlighter(QTextDocument* parent = nullptr);

  protected:
    void highlightBlock(const QString& text) override;

  private:
    struct HighlightingRule
    {
      QRegularExpression pattern;
      QTextCharFormat    format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat valueFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat quoteFormat;
    QTextCharFormat errorFormat;
};

#endif // MPVDECK_MPVCONFHIGHLIGHTER_H
