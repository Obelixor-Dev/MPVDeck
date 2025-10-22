#ifndef MPVDECK_CONFIGRESULT_H
#define MPVDECK_CONFIGRESULT_H

#include <QString>

namespace MPVDeck {

struct ConfigResult {
  bool success;
  QString errorMessage;

  static auto successResult() -> ConfigResult { return {.success = true, .errorMessage = ""}; }
  static auto failureResult(const QString &message) -> ConfigResult {
    return {.success = false, .errorMessage = message};
  }
};

} // namespace MPVDeck

#endif // MPVDECK_CONFIGRESULT_H
