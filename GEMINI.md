# MPVDeck Project Overview for Gemini

This document provides a comprehensive overview of the MPVDeck project, intended to serve as instructional context for future interactions with the Gemini AI agent.

## 1. Project Overview

**MPVDeck** is a user-friendly C++ Qt6 GUI application designed for configuring the `mpv` video player. It aims to provide a graphical interface for managing various `mpv` settings, making it easier for users to customize their playback experience.

*   **Primary Goal:** To create a user-friendly C++ Qt6 GUI application for configuring the `mpv` video player.
*   **Main Technologies:** C++, Qt6, CMake.
*   **Architecture:** The project follows the Model-View-ViewModel (MVVM) architectural pattern to ensure a clean separation of concerns:
    *   **Model (`ConfigManager`):** Responsible for reading, parsing, and writing `mpv` configuration files (`mpv.conf`).
    *   **ViewModel (`SettingsViewModel`):** Acts as a bridge between the Model and the View, containing the application's presentation logic and state for settings.
    *   **View (`SettingsView`):** The graphical user interface built with Qt6, responsible for displaying data from the ViewModel and forwarding user interactions.
*   **Target Platform:** Linux desktops, with a long-term goal of distribution via Flathub.

## 2. Building and Running

The project uses CMake as its build system.

### Build Commands:

To build the project, navigate to the project root and execute the following commands:

```bash
cmake -B build
cmake --build build
```

Alternatively, if you have `make` installed, you can use:

```bash
cmake -B build
cd build
make -j$(nproc) # Uses all available CPU cores for faster compilation
```

### Running the Application:

After a successful build, the executable will be located in the `build` directory.

```bash
./build/MPVDeck
```

### Testing:

The project aims for automated testing using the Qt Test framework.

```bash
# TODO: Add specific commands for running tests once implemented.
```

## 3. Development Conventions

### Code Style:

*   **Enforced using `clang-format`:** The project adheres to a consistent code style, configured via the `.clang-format` file (LLVM style).

### Code Quality:

*   **`clang-tidy` integration:** `clang-tidy` is used to improve code quality and identify potential issues, configured via the `.clang-tidy` file.

### Version Control:

*   **Git:** The project uses Git for version control, with a `.gitignore` file to exclude unnecessary files from the repository.

### Documentation:

*   The project aims for comprehensive documentation, including `README.md`, `CONTRIBUTING.md`, and in-code documentation (Doxygen-compatible).
*   `docs/PROJECT_OBJECTIVES.md` outlines the initial goals and architectural decisions.
*   `docs/MPV_OPTIONS.md` provides a detailed list of MPV options, their possible values, and descriptions, which are reflected in the application's settings UI.

### Development Environment:

*   **IDE:** CLion is the primary Integrated Development Environment used for this project.

## 4. Key Functionalities Implemented

The application provides a comprehensive settings interface for `mpv`, categorized into:

*   **Audio Settings:** Mute, Volume, Audio Device, Audio Channels, Audio Delay, Audio Normalize Downmix.
*   **Video Settings:** Video Output (`vo`), Profile, Scale, Chroma Upscaling (`cscale`), Downscaling (`dscale`), Interpolation, Temporal Scaling (`tscale`), Video Sync, Debanding.
*   **Subtitles Settings:** Visibility, Font Size, Color, Languages, Auto-load, Subtitle ID, Forced Subtitles Only, Subtitle Font.
*   **Playback & Behavior:** Resume Playback, Save Position on Quit, Loop File, Keep Open, Autofit Larger, YTDL Raw Options.
*   **Performance / Caching:** Cache, Cache Duration, Demuxer Max Bytes, Hardware Decoding (`hwdec`), Hardware Decoding Codecs.
*   **Interface / OSD:** OSD Level, OSD Font Size, OSD Duration, On-Screen Controller (`osc`), Screenshot Format, Screenshot Directory, Screenshot Template.

The application also includes a notification system to inform the user about the success or failure of saving settings.
