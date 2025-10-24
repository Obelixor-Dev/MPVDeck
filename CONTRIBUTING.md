# Contributing to MPVDeck

Thank you for your interest in contributing to MPVDeck! We welcome contributions from everyone.

## How to Contribute

1.  Fork the repository.
2.  Create a new branch for your feature or bug fix.
3.  Make your changes.
4.  Ensure your code adheres to the project's coding style (enforced by `.clang-format`).
5.  Write clear, concise commit messages.
6.  Submit a pull request.

## Code Style

This project uses `clang-format` to maintain a consistent code style. Please ensure your contributions are formatted using `clang-format` before submitting a pull request.

### Running Code Quality Tools
- Format code: `cmake --build build --target format`
- Run static analysis: `cmake --build build --target clang-tidy-check`
Ensure both pass before submitting a pull request.

## Code Quality

This project uses `clang-tidy` for static analysis to enforce code quality and identify potential issues. When building the project with CMake, `clang-tidy` is automatically run with a specific set of checks. You can configure the checks via the `.clang-tidy` file.

## Submitting Contributions

All contributions must be licensed under the MIT License to ensure compatibility with the project. By submitting a pull request, you agree to license your work under the MIT License.

## Feature Requests

For feature requests, please open an issue to discuss the new feature. This helps ensure that the feature aligns with the project's goals and avoids duplicate work.
