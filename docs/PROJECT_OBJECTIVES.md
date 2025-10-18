# Project Objectives: MPVDeck

This document outlines the initial goals and architectural decisions for the MPVDeck project.

## 1. Core Concept

*   **Project Name:** MPVDeck
*   **Goal:** To create a user-friendly C++ Qt6 GUI application for configuring the `mpv` video player.
*   **Primary Target:** Linux desktops, distributed via Flathub.

## 2. Architecture & Design

*   **Core Pattern:** Model-View-ViewModel (MVVM) to ensure a clean separation of concerns.
    *   **Model:** Responsible for reading, parsing, and writing `mpv` configuration files. It will represent the raw configuration data.
    *   **ViewModel:** Acts as a bridge between the Model and the View. It will contain the application's presentation logic and state.
    *   **View:** The graphical user interface built with Qt6. It will be responsible for displaying the data from the ViewModel and forwarding user interactions.
*   **Development IDE:** CLion

## 3. Long-Term & Open Source Goals

The project aims to be a high-quality, maintainable open-source application. This will be achieved through:

*   **Consistent Code Style:** Enforced using `clang-format`.
*   **Modern Build System:** Using CMake for project configuration and dependency management.
*   **Automated Testing:** A comprehensive test suite using the Qt Test framework to ensure stability and prevent regressions.
*   **Continuous Integration (CI):** Setting up a CI pipeline (e.g., GitHub Actions) to automate builds and testing.
*   **Comprehensive Documentation:**
    *   `README.md`: For project overview and build instructions.
    *   `CONTRIBUTING.md`: For contributor guidelines.
    *   In-code documentation (Doxygen-compatible) for API reference.

## 4. Distribution

*   **Primary Distribution Platform:** Flathub.
*   **Packaging:** A Flatpak manifest will be created to handle the build, packaging, and sandboxing requirements.
