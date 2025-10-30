# Flathub Submission Checklist for MPVDeck

This checklist outlines the steps required to prepare and submit MPVDeck to Flathub.

## Preparation

- [ ] **Format Source Files:**
    - [ ] Run `clang-format` on all C++ source files to ensure consistent code style.

- [ ] **Update the Manifest for Flathub (`org.mpvdeck.MPVDeck.yml`):**
    - [ ] Change `sources` to use an `archive` type with a remote `url` (e.g., GitHub release tarball) and `sha256` checksum.
    - [ ] **NOTE:** `sources` was temporarily reverted to `type: dir, path: .` for local testing. **ACTION REQUIRED:** Revert this change and update `url` and `sha256` with the actual release tarball details before submission.
    - [ ] Add the missing permission: `- --filesystem=xdg-config/mpv`.
    - [ ] Verify if `--filesystem=xdg-config/mpv:create` is needed.
    - [ ] Update `runtime-version` if a newer KDE Platform is available.
    - [ ] Ensure build commands correctly install the desktop file, icon, and other assets.

- [ ] **Validate Metainfo (`org.mpvdeck.MPVDeck.metainfo.xml`):**
    - [ ] Install `appstream` (e.g., `sudo apt install appstream`).
    - [ ] Run `appstream-util validate org.mpvdeck.MPVDeck.metainfo.xml`.
    - [ ] Fix any warnings or errors (e.g., description formatting, screenshot resolution, icon squareness).
    - [x] **ACTION REQUIRED:** Re-enable and correctly configure screenshots in `org.mpvdeck.MPVDeck.metainfo.xml` (e.g., by including them in the build and referencing locally).
    - [ ] Add content rating if applicable (unlikely for MPVDeck).

- [x] **Local Testing:**
    - [x] Build the Flatpak locally using the updated manifest: `flatpak-builder --force-clean build-dir org.mpvdeck.MPVDeck.yml`.
    - [x] Install the built Flatpak: `flatpak install --user build-dir/org.mpvdeck.MPVDeck.flatpak`.
    - [x] Run the Flatpak: `flatpak run org.mpvdeck.MPVDeck`.
    - [x] Test on multiple Linux distributions (e.g., Fedora, Ubuntu).
    - [x] Verify that config saving and loading works correctly within the sandbox.
    - [x] Ensure no crashes or UI scaling issues.
    - [ ] **NOTE:** Since config saving worked, `--filesystem=xdg-config/mpv` is sufficient. `--filesystem=xdg-config/mpv:create` is not explicitly needed unless the directory is guaranteed not to exist.

## Flathub Submission

- [ ] **Fork Flathub Repository:**
    - [ ] Fork `https://github.com/flathub/flathub`.

- [x] **Create Manifest File in Fork:**
    - [x] In your forked repository, create `org.mpvdeck.MPVDeck.yaml` (note the `.yaml` extension) in the root directory.
    - [x] Copy your updated Flatpak manifest content into this new `.yaml` file.

- [x] **Commit and Push Changes:**
    - [x] Commit the new manifest file to your forked repository.
    - [x] Push the changes to your GitHub fork.

- [ ] **Open a Pull Request (PR) to Flathub:**
    - [ ] *Before opening the PR, ensure a release has been tagged on GitHub and a source tarball is available.*
    - [ ] Open a PR against `flathub/flathub`'s `new-pr` branch.
    - [ ] Set the PR title to "New App: MPVDeck".
    - [ ] In the PR description, include:
        - [ ] Link to the MPVDeck project repository and the specific release.
        - [ ] A brief explanation of the application.
        - [ ] Confirmation that the app is open-source and MIT-licensed.
        - [ ] Justification for any special permissions (e.g., `xdg-config/mpv`).
        - [ ] Instructions for reviewers to test the application.

- [ ] **Review and Iteration:**
    - [ ] Respond promptly to feedback and requests for changes from Flathub reviewers.
    - [ ] Make necessary adjustments to the manifest or code as requested.

## Post-Submission

- [ ] **Update Documentation:**
    - [ ] Update `README.md` with instructions for installing MPVDeck from Flathub (e.g., `flatpak install flathub org.mpvdeck.MPVDeck`).
    - [ ] Monitor for issues on GitHub and Flathub.
    - [ ] Plan for future releases and updates to Flathub.
