
# MPVDeck

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

MPVDeck is a user-friendly C++ Qt6 GUI application designed for configuring the `mpv` video player. It aims to provide a graphical interface for managing various `mpv` settings, making it easier for users to customize their playback experience.

## License

MPVDeck is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Building and Running

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

## Usage

MPVDeck provides a GUI to configure mpv settings:
1. **Tabs**: Navigate tabs (Audio, Video, Subtitles, etc.) to adjust settings.
2. **Raw Config Editor**: Edit `mpv.conf` directly with syntax highlighting.
3. **Save**: Click "Save" to write changes to `~/.config/mpv/mpv.conf`.

![Audio Tab](screenshots/audio_tab.png)
![Raw Config Editor](screenshots/raw_editor.png)

### Flatpak Installation:

MPVDeck can be installed as a Flatpak application.

1.  **Build and Install Locally (for testing/development):**
    ```bash
    flatpak-builder --force-clean build-dir org.mpvdeck.MPVDeck.yml
    flatpak install --user build-dir org.mpvdeck.MPVDeck
    ```
2.  **Run the Flatpak Application:**
    ```bash
    flatpak run org.mpvdeck.MPVDeck
    ```
3.  **Install from Flathub (once available):**
    ```bash
    flatpak install flathub org.mpvdeck.MPVDeck
    ```
