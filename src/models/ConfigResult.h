#ifndef MPVDECK_CONFIGRESULT_H
#define MPVDECK_CONFIGRESULT_H

#include <QString>

namespace MPVDeck {

struct ConfigResult {
    bool success;
    QString errorMessage;

    static ConfigResult successResult() { return {true, ""}; }
    static ConfigResult failureResult(const QString &message) { return {false, message}; }
};

} // namespace MPVDeck

#endif // MPVDECK_CONFIGRESULT_H
