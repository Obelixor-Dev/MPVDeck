#include "MpvConfHighlighter.h"
#include <QtCore/QRegularExpression>

MpvConfHighlighter::MpvConfHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
  // Keywords (mpv options)
  keywordFormat.setForeground(Qt::blue);
  keywordFormat.setFontWeight(QFont::Bold);

  // Values
  valueFormat.setForeground(Qt::darkGreen);

  // Comments
  commentFormat.setForeground(Qt::darkGray);
  commentFormat.setFontItalic(true);
  highlightingRules.append({QRegularExpression("#.*"), commentFormat});

  // Quoted values
  quoteFormat.setForeground(Qt::darkRed);
  highlightingRules.append(
      {QRegularExpression("\"([^\\\"]|\\\\.)*\""), quoteFormat});

  // Error highlighting (for lines marked with # ERROR: from ConfigManager)
  errorFormat.setForeground(Qt::red);
  errorFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
  errorFormat.setUnderlineColor(Qt::red);
}

void MpvConfHighlighter::highlightBlock(const QString& text)
{
  // Highlight error lines first to override other highlighting
  if(text.startsWith("# ERROR:", Qt::CaseInsensitive))
  {
    setFormat(0, text.length(), errorFormat);
    return;
  }

  // Apply highlighting rules
  for(const HighlightingRule& rule : highlightingRules)
  {
    QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
    while(i.hasNext())
    {
      QRegularExpressionMatch match = i.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }

  // Highlight key-value pairs
  QRegularExpression      keyValueRx("^\\s*([^=\\s]+)\\s*=\\s*(.*)");
  QRegularExpressionMatch keyValueMatch = keyValueRx.match(text);
  if(keyValueMatch.hasMatch())
  {
    // Highlight key
    setFormat(keyValueMatch.capturedStart(1), keyValueMatch.capturedLength(1),
              keywordFormat);
    // Highlight value
    setFormat(keyValueMatch.capturedStart(2), keyValueMatch.capturedLength(2),
              valueFormat);
  }
  else
  {
    // Highlight standalone keys (flags)
    QRegularExpression      flagRx("^\\s*([^=\\s]+)\\s*$");
    QRegularExpressionMatch flagMatch = flagRx.match(text);
    if(flagMatch.hasMatch())
    {
      setFormat(flagMatch.capturedStart(1), flagMatch.capturedLength(1),
                keywordFormat);
    }
  }
}
