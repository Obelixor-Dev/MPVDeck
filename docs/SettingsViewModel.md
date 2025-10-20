# SettingsViewModel

The `SettingsViewModel` class is a central component in the MPVDeck application. It acts as the main ViewModel in the MVVM architecture, orchestrating the interaction between the `ConfigManager` (Model) and the various specialized ViewModels for different settings categories.

## Responsibilities

-   **Initializes and manages all other ViewModels:** It creates instances of `AudioViewModel`, `VideoViewModel`, `SubtitleViewModel`, `PlaybackBehaviorViewModel`, `PerformanceCachingViewModel`, and `InterfaceOsdViewModel`.
-   **Connects signals and slots:** It connects the `...Changed` signals from each of the specialized ViewModels to its own `setIsDirty` slot. This allows the application to track whether any settings have been modified.
-   **Loads and saves settings:** It provides the `loadSettings` and `saveSettings` methods to read from and write to the `mpv.conf` file via the `ConfigManager`.
-   **Loads default settings:** The `loadDefaults` method resets all settings to their default values as defined in `defaults.json`.
-   **Provides raw configuration access:** The `getRawConfig` and `applyRawConfig` methods allow for direct access to the raw text of the `mpv.conf` file.
-   **Provides option descriptions:** The `getOptionDescription` method retrieves the description for a given setting from `defaults.json`.

## Key Methods

-   `loadSettings()`: Reads the `mpv.conf` file and updates the properties of all the specialized ViewModels.
-   `saveSettings()`: Collects the current values from all the specialized ViewModels and saves them to the `mpv.conf` file.
-   `loadDefaults()`: Parses the `defaults.json` file and sets the properties of all the specialized ViewModels to their default values.

## Interactions

-   **`ConfigManager`:** The `SettingsViewModel` uses the `ConfigManager` to read and write the `mpv.conf` file.
-   **Specialized ViewModels:** The `SettingsViewModel` owns and manages the lifecycle of the other ViewModels. It accesses their properties to load and save settings.
-   **`SettingsView`:** The `SettingsView` (the UI) binds to the properties of the `SettingsViewModel` and its child ViewModels to display and edit the settings.
