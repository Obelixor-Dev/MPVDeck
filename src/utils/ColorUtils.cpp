#include "ColorUtils.h"
#include <QColor>
#include <QRegularExpression>

namespace ColorUtils
{
QString normalizeColor(const QString& colorInput)
{
  QColor color(colorInput);

  if(color.isValid())
  {
    return color.name(QColor::HexRgb).toUpper();
  }

  // Try to parse common formats like rgb(r,g,b)
  QRegularExpression rgbRegex(R"_(rgb((\d{1,3}),\s*(\d{1,3}),\s*(\d{1,3})))_");
  QRegularExpressionMatch match = rgbRegex.match(colorInput);
  if(match.hasMatch())
  {
    int r = match.captured(1).toInt();
    int g = match.captured(2).toInt();
    int b = match.captured(3).toInt();
    color.setRgb(r, g, b);
    if(color.isValid())
    {
      return color.name(QColor::HexRgb).toUpper();
    }
  }

  return {}; // Return empty string for invalid colors
}
} // namespace ColorUtils
