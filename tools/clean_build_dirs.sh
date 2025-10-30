#!/bin/bash
# This script cleans all build-related directories for the MPVDeck project.
# Run this script from the project root directory.

echo "Cleaning CMake build directories..."
rm -rf build build-debug build-release

echo "Cleaning Flatpak build directories..."
rm -rf build-dir .flatpak-builder

echo "All build directories have been cleaned."
