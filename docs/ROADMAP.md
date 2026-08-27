# NX Browser roadmap

## Milestone 0 — repository/toolchain
- [x] Create project repository
- [x] Define architecture and scope
- [ ] Validate devkitPro/libnx build in CI
- [ ] Produce first `.nro` artifact

## Milestone 1 — native shell
- [ ] 1280x720 UI
- [ ] Touch input
- [ ] Joy-Con/controller input
- [ ] Address bar
- [ ] On-screen keyboard integration
- [ ] Settings and persistent storage

## Milestone 2 — browser engine
- [ ] Select a maintainable Switch-compatible web engine
- [ ] HTTPS/TLS
- [ ] HTML/CSS rendering
- [ ] JavaScript
- [ ] Images/audio/video where technically supported

## Milestone 3 — browser features
- [ ] Tabs
- [ ] History
- [ ] Bookmarks
- [ ] Downloads
- [ ] Private browsing mode
- [ ] Error/offline pages

## Milestone 4 — performance and compatibility
- [ ] Memory management
- [ ] Caching
- [ ] GPU acceleration where available
- [ ] WebAssembly/WebGL where available
- [ ] Test against modern sites

## Important constraint
A full Chromium/Blink port is a major systems project. We will not label a UI wrapper as Chromium. The engine milestone will be implemented and tested separately.
