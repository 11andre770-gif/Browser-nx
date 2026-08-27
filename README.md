# NX Browser

Standalone Nintendo Switch homebrew browser project.

## Goal
A separate browser-like application for Nintendo Switch Lite running modern Atmosphere/Kefir. The project is intended to evolve toward a capable browser UI with touch/controller input, tabs, bookmarks, history, downloads, HTTPS and a Switch-compatible web engine.

## Current status
Early project bootstrap. The first milestone is a buildable homebrew shell and CI/toolchain validation. This repository intentionally does not claim to contain a full Chromium/Blink port yet.

## Architecture
- Switch application layer: libnx/devkitPro
- Browser UI: native Switch rendering/input layer
- Web engine: to be evaluated and integrated in a later milestone
- CI: GitHub Actions for reproducible builds

## License
MIT for original project code. Third-party components retain their own licenses.
