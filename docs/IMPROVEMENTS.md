### ✅ Already Done (Strengths from Review):

*   Clean MVVM structure with good separation of concerns.
*   Consistent naming conventions.
*   Modern C++17 standard and clean CMake configuration.
*   Correct application of Qt idioms (signals/slots, Q_PROPERTY, QObject).
*   Uses Qt6::Widgets, avoiding deprecated APIs.
*   Clean, minimal, and portable CMake configuration.
*   Correct C++ standard enforcement in CMake.
*   Proper use of `AUTOUIC`, `AUTOMOC`, `AUTORCC` in CMake.
*   `ConfigManager` correctly uses `QStandardPaths` and fallbacks.
*   `ConfigManager` handles comments, inline comments, and whitespace.
*   `ConfigManager` avoids undefined behavior (graceful returns).
*   `ConfigManager` has simple and robust parsing logic.
*   `SettingsViewModel` has thorough mapping of `mpv.conf` settings to properties.
*   `SettingsViewModel` uses Qt properties and signals for real-time binding.
*   `SettingsViewModel` has reasonable defaults and clear initialization.
*   `SettingsViewModel` has clear naming and strong separation from the UI.
*   `SettingsView` has clear, modular creation of UI tabs with well-labelled controls.
*   `SettingsView` has bidirectional binding between UI elements and ViewModel.
*   `SettingsView` has logical grouping by feature domain.
*   `SettingsView` uses layout managers properly.
*   `main.cpp` has a custom `messageHandler` for Qt logs.
*   `main.cpp` has a clean setup of ViewModel and View.
*   `main.cpp` uses RAII (`QApplication`).
*   `main.cpp` uses `qInstallMessageHandler` for consistent logging.
*   `tests/` directory exists (though empty).
*   `docs/PROJECT_OBJECTIVES.md` and `docs/MPV_OPTIONS.md` are excellent.

---

### 📝 To-Do Checklist (Improvements Suggested by Review):

#### 🧱 Overall Architecture
*   [x] Introduce unit tests for `ConfigManager` and `SettingsViewModel` logic. (ConfigManager tests are already present)
*   [ ] Consider dependency injection for `ConfigManager`.
*   [ ] Evaluate `QSettings` or `QJsonDocument` for structured config persistence.

#### ⚙️ CMakeLists.txt
*   [x] Add `include_directories(src)` or use `target_include_directories`.
*   [x] Split CMake into subdirectories for larger projects (e.g., `add_subdirectory(src/models)`).
*   [x] Add `target_compile_features(MPVDeck PRIVATE cxx_std_17)` for explicit clarity.

#### 📄 ConfigManager
*   [x] Implement comment preservation when saving `mpv.conf`.
*   [x] Improve error propagation (return structured error info instead of just warnings). (Partial: `saveConfigFile` now returns bool)
*   [x] Explicitly set `QTextStream` encoding to UTF-8.
*   [x] Trim trailing newlines and handle duplicate keys in `readConfigFile()` (Partial: Newlines handled by `parseLine`/`serializeLine`. Duplicate keys in `getSettingsMap()` follow `QMap` behavior (last value wins). `saveConfigFile()` updates the first occurrence of a key if duplicates exist in the original file.)
*   [x] Enhance `mpv.conf` existence check: `if (mpvConfigDir.exists("mpv.conf") && QFile::exists(mpvConfigDir.filePath("mpv.conf")))`.

#### 🧠 SettingsViewModel
*   [x] Refactor `SettingsViewModel` by splitting into smaller, domain-specific view models (e.g., `AudioViewModel`, `VideoViewModel`, `SubtitleViewModel`, `PlaybackBehaviorViewModel`, `PerformanceCachingViewModel`, `InterfaceOsdViewModel` extracted)
*   [x] Unify duplicated "Subtitles" sections.
*   [x] Implement partial updates for `saveSettings()` instead of overwriting the entire map.
*   [x] Introduce input validation (e.g., range checks for `volume`, `osd-level`). (Partial: Volume, OSD Level, and OSD Font Size now have range checks)
*   [x] Review `DEFAULT_SUBTITLE_FONT_SIZE` (current 55, consider 28).
*   [ ] Explore dynamic generation of `loadSettings()` and `saveSettings()` from metadata. (Future Consideration)

#### 🪟 SettingsView
*   [ ] Modularize `SettingsView` by extracting tab builders into helper methods (e.g., `createAudioTab()`).
*   [ ] Add error or confirmation dialogs for saving settings.
*   [ ] Implement "Save" / "Revert" buttons for UI persistence.
*   [ ] Add tooltips with MPV option descriptions to UI controls.
*   [ ] Consider using `QFormLayout` for better readability in some sections.
*   [ ] Abstract minor UI duplication (e.g., label/spinbox pairs).

#### 🧩 main.cpp
*   [ ] Add support for CLI arguments (e.g., `--verbose`, `--config`) using `QCommandLineParser`.

#### 🧪 Testing and Extensibility
*   [x] Write unit tests for `ConfigManager::readConfigFile()`.
*   [ ] Write unit tests for `SettingsViewModel::loadSettings()` mapping logic.
*   [x] Integrate Qt Test framework for automated testing.

#### 🧾 Documentation
*   [x] Create a `README.md` file with build/run instructions.
*   [x] Create a `CONTRIBUTING.md` file.
*   [x] Add a `LICENSE` file.
