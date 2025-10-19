# MPVDeck

MPVDeck is a user-friendly C++ Qt6 GUI application designed for configuring the `mpv` video player. It aims to provide a graphical interface for managing various `mpv` settings, making it easier for users to customize their playback experience.

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
