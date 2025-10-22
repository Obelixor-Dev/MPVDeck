#ifndef MPVDECK_CONSTANTS_H
#define MPVDECK_CONSTANTS_H

namespace MPVDeck::Constants {

// UI related constants
constexpr int MAX_VOLUME = 200;
constexpr double AUDIO_DELAY_SINGLE_STEP = 0.1;
constexpr int MAX_SUBTITLE_FONT_SIZE = 200;
constexpr int MAX_OSD_LEVEL = 3;
constexpr int MIN_OSD_LEVEL = 0;
constexpr int MIN_OSD_FONT_SIZE = 10;
constexpr int MAX_OSD_FONT_SIZE = 100;
constexpr int MAX_CACHE_SECS = 600; // 10 minutes
constexpr int MIN_OSD_DURATION = 0;
constexpr int MAX_OSD_DURATION = 10000;
constexpr int DEFAULT_FONT_SIZE = 9;
constexpr int DEFAULT_OSD_FONT_SIZE = 28;
constexpr int DEFAULT_OSD_DURATION = 2000;
constexpr int DEFAULT_CACHE_SECS = 10;
constexpr double MIN_AUDIO_DELAY = -100.0;
constexpr double MAX_AUDIO_DELAY = 100.0;

} // namespace Constants

#endif // MPVDECK_CONSTANTS_H