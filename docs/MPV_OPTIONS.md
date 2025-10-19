This is the Gemini CLI. We are setting up the context for our chat.
Today's date is Sunday, 19 October 2025 (formatted according to the user's locale).
My operating system is: linux
I'm currently working in the directory: /home/skum/Dev/CPP/MPVDeck
Here is the folder structure of the current working directories:

Showing up to 200 items (files + folders). Folders or files indicated with ... contain more items not shown, were ignored, or the display limit (200 items) was reached.

/home/skum/Dev/CPP/MPVDeck/
├───.clang-format
├───.clang-tidy
├───.gitignore
├───CMakeLists.txt
├───.git/...
├───.idea/
├───build/
│   ├───.qt/...
│   ├───CMakeFiles/...
│   └───MPVDeck_autogen/...
├───cmake-build-clang-tidy/
│   ├───.qt/...
│   ├───CMakeFiles/...
│   └───MPVDeck_autogen/...
├───cmake-build-debug/
│   ├───.cmake/...
│   ├───.qt/...
│   ├───CMakeFiles/...
│   ├───MPVDeck_autogen/...
│   └───Testing/...
├───docs/
│   └───PROJECT_OBJECTIVES.md
├───resources/
├───src/
│   ├───main.cpp
│   ├───models/
│   │   ├───ConfigManager.cpp
│   │   └───ConfigManager.h
│   ├───viewmodels/
│   │   ├───SettingsViewModel.cpp
│   │   └───SettingsViewModel.h
│   └───views/
│       ├───SettingsView.cpp
│       └───SettingsView.h
└───tests/

Reminder: Do not return an empty response when a tool call is required.

My setup is complete. I will provide my first command in the next turn.Ah — got it! You don’t just want the common options, you also want **the valid values for each option** (e.g., for `vo`, valid outputs; for `hwdec`, the supported hardware decoding modes, etc.). That’s a bit more detailed.

Here’s a **Linux-focused list of most-used MPV options along with their possible values and defaults**:

---

## 1️⃣ Video settings

| Option          | Possible Values                                                       | Default            | Description                |
| --------------- | --------------------------------------------------------------------- | ------------------ | -------------------------- |
| `vo`            | `gpu`, `gpu-next`, `x11`, `xv`, `opengl`, `sdl`                       | `gpu`              | Video output driver        |
| `profile`       | `gpu-hq`, `gpu`, `low-latency`, `lavfi-complex`, `default`            | `gpu-hq`           | Preset video options       |
| `scale`         | `bilinear`, `bicubic`, `lanczos`, `ewa_lanczossharp`, `spline36`, ... | `ewa_lanczossharp` | Upscaling filter           |
| `cscale`        | Same as `scale`                                                       | `ewa_lanczossharp` | Chroma upscaling           |
| `dscale`        | `bilinear`, `bicubic`, `lanczos`, `mitchell`                          | `mitchell`         | Downscaling filter         |
| `interpolation` | `yes`, `no`                                                           | `no`               | Enable frame interpolation |
| `tscale`        | `oversample`, `linear`, `nearest`                                     | `oversample`       | Temporal scaling           |
| `video-sync`    | `display-resample`, `audio`, `video`                                  | `display-resample` | Audio/video sync method    |
| `deband`        | `yes`, `no`                                                           | `no`               | Apply debanding filter     |

---

## 2️⃣ Audio settings

| Option                    | Possible Values                           | Default  | Description                 |
| ------------------------- | ----------------------------------------- | -------- | --------------------------- |
| `audio-device`            | `auto`, `alsa`, `pulse`, `jack`, `oss`    | `auto`   | Audio output device         |
| `volume`                  | `0–200`                                   | `100`    | Default volume              |
| `audio-channels`          | `stereo`, `mono`, `5.1`, `7.1`, `downmix` | `stereo` | Audio channel layout        |
| `audio-delay`             | `double`                                  | `0.0`    | Audio delay in seconds      |
| `audio-normalize-downmix` | `yes`, `no`                               | `yes`    | Normalize volume on downmix |

---

## 3️⃣ Subtitles

| Option            | Possible Values                         | Default   | Description                            |
| ----------------- | --------------------------------------- | --------- | -------------------------------------- |
| `no-sub`          | `yes`, `no`                             | `no`      | Disable all subtitles (corresponds to UI's `sub-visibility` inverted) |
| `sub-auto`        | `no`, `fuzzy`, `exact`                  | `fuzzy`   | Auto-load subtitle files               |
| `sid`             | `no`, `0`, `1`, ...                     | `0`       | Subtitle track selection (`no` = none) |
| `slang`           | Language codes (e.g., `en`, `fr`, `de`) | empty     | Preferred subtitle language            |
| `sub-forced-only` | `yes`, `no`                             | `no`      | Show only forced subtitles             |
| `sub-font`        | Any font installed on system            | `"Sans"`  | Subtitle font                          |
| `sub-font-size`   | Integer                                 | `36`      | Subtitle font size                     |
| `sub-color`       | Hex colour (e.g., `#FFFFFF`)            | `#FFFFFF` | Subtitle colour                        |

---

## 4️⃣ Playback & behavior

| Option                  | Possible Values              | Default | Description                          |
| ----------------------- | ---------------------------- | ------- | ------------------------------------ |
| `resume-playback`       | `yes`, `no`                  | `no`    | Resume last position                 |
| `save-position-on-quit` | `yes`, `no`                  | `no`    | Save position when quitting          |
| `loop-file`             | `yes`, `no`, `inf`           | `no`    | Loop current file                    |
| `keep-open`             | `yes`, `no`, `always`        | `no`    | Keep window open after playback ends |
| `autofit-larger`        | `%`, pixels, e.g., `90%x90%` | -       | Resize window automatically          |

---

## 5️⃣ Performance / caching

| Option              | Possible Values                                                                 | Default     | Description                           |
| ------------------- | ------------------------------------------------------------------------------- | ----------- | ------------------------------------- |
| `cache`             | `yes`, `no`                                                                     | `yes`       | Enable input/network cache            |
| `cache-secs`        | Integer seconds                                                                 | `10`        | Cache duration                        |
| `demuxer-max-bytes` | Bytes, e.g., `128MiB`                                                           | `auto`      | Max buffer for demuxer                |
| `hwdec`             | `auto`, `auto-safe`, `vaapi`, `nvdec`, `vdpau`, `dxva2`, `videotoolbox`, `none` | `auto-safe` | Hardware decoding mode                |
| `hwdec-codecs`      | `all`, `h264`, `hevc`, `vp9`, etc.                                              | `all`       | Which codecs to allow for HW decoding |

---

## 6️⃣ Interface / OSD

| Option                 | Possible Values     | Default | Description            |
| ---------------------- | ------------------- | ------- | ---------------------- |
| `osd-level`            | `0–3`               | `1`     | OSD verbosity level    |
| `osd-font-size`        | Integer             | `28`    | Font size for OSD      |
| `osd-duration`         | ms                  | `2000`  | Duration messages stay |
| `osc`                  | `yes`, `no`         | `yes`   | On-screen controller   |
| `screenshot-format`    | `png`, `jpg`, `bmp` | `png`   | Screenshot format      |
| `screenshot-directory` | Path                | `~`     | Screenshot folder      |
| `screenshot-template`  | `%F_%P`             | `%F_%P` | File naming template   |

---

### 🔗 Optional online/video options

| Option             | Possible Values                                            | Default | Description                              |
| ------------------ | ---------------------------------------------------------- | ------- | ---------------------------------------- |
| `ytdl-raw-options` | `sub-lang=`, `write-auto-sub=`, `write-sub=`, `subtitles=` | empty   | Prevent YouTube auto-generated subtitles |

---