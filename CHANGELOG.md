# CHANGELOG

## Version 1.0.0 - 2025-10-24

### Added
- Initial release of MPVDeck, a GUI for configuring mpv video player.
- Comprehensive settings interface for Audio, Video, Subtitles, Playback & Behavior, Performance / Caching, and Interface / OSD.
- Model-View-ViewModel (MVVM) architecture for clear separation of concerns.
- Configuration management for reading, parsing, and writing `mpv.conf`.
- Notification system for save success/failure.

### Changed
- Updated Flatpak manifest for Flathub submission readiness.
- Integrated metainfo file (`org.mpvdeck.MPVDeck.metainfo.xml`).
- Resized application icon to 512x512.
- Improved error handling with user-facing messages for configuration save failures.

### Fixed
- Resolved various Flatpak linting issues.
