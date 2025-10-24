# Flatpak Integration To-Do List

This checklist outlines the remaining tasks and issues to address for a complete Flatpak integration and Flathub submission.

- [ ] **Investigate `desktop-app-launchable-missing` error:**
  - **Reason:** This error indicates that the Flatpak runtime is having trouble finding or launching the application via its desktop file. Although `Exec` path and `StartupWMClass` have been set, the issue persists. Further debugging might be needed to understand why the desktop environment isn't correctly associating the application with its launchable.

- [ ] **Address `appid-url-not-reachable`:**
  - **Reason:** The `https://mpvdeck.org` URL specified in the metainfo file is currently unreachable. This is expected as the project is new. Once the official website for MPVDeck is live, this URL should be updated or verified to be reachable.

- [ ] **Address `appstream-missing-screenshots` and `appstream-screenshots-not-mirrored-in-ostree`:**
  - **Reason:** The metainfo file references external screenshot URLs that are not mirrored within the Flatpak repository. For a production Flathub submission, it's recommended to either host these screenshots within the repository or ensure they are mirrored by Flathub's infrastructure. This also implies that actual screenshots of the application should be created and linked.

- [ ] **Address `manifest-json-warnings`:**
  - **Reason:** This is a generic warning about deserialization of the "metadata" property in the manifest. While not critical, it's good practice to resolve all warnings to ensure full compliance and avoid potential future issues. This might resolve itself once other issues are fixed or might require a more specific adjustment to the manifest structure.
