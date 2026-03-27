# ADR-006: Version Management Strategy

**Date:** 2026-02-11  
**Updated:** 2026-03-27  
**Status:** Accepted (Amended)  
**Author:** Philippe Andersson

## Context

Ratatoskr consists of two executables (main app and SharePlugin) that need to log their version at startup for debugging and support purposes. We need a consistent strategy for version management that:

1. Works for both executables
2. Is maintained in a single authoritative source
3. Is automatically generated from git tags
4. Is accessible at runtime for logging

## Current State

The root `CMakeLists.txt` already implements version generation:

- Uses `git describe` to extract version from git tags
- Defines `BUILD_VERSION` as a compiler flag (`-DBUILD_VERSION="${BUILD_VERSION}"`)
- Tagged commits use the tag as version (e.g., `0.0.1`)
- Untagged commits use `{last-tag}.{timestamp}` format

However, this `BUILD_VERSION` define is only available to the main app sources, not to the SharePlugin subdirectory.

## Decision

We will use the existing CMake-based version generation with the following strategy:

### 1. Single Source of Truth
- Git tags remain the authoritative version source
- Root `CMakeLists.txt` continues to generate `BUILD_VERSION`
- No manual version strings in source files

### 2. SharePlugin Access to Version
The `BUILD_VERSION` preprocessor define will be made available to the SharePlugin by:

- The root CMakeLists.txt already defines it globally via `add_definitions(-DBUILD_VERSION="${BUILD_VERSION}")`
- This makes it available to all targets, including the SharePlugin subdirectory
- No additional CMake changes needed

### 3. Runtime Logging
Both executables will log their version at startup using:

```cpp
qDebug() << "Application Name" << BUILD_VERSION << "starting...";
```

The `BUILD_VERSION` macro will be available as a string literal at compile time.

### 4. Version Format
- Release builds (tagged commits): `X.Y.Z` (e.g., `0.0.1`)
- Development builds: `X.Y.Z.YYMMDDHHMMSS` (e.g., `0.0.1.260211153000`)

## Implementation

1. Main app (`ratatoskr/main.cpp`):
   - Add `qDebug()` statement immediately after `QGuiApplication` initialization
   - Format: `"Ratatoskr" BUILD_VERSION "starting..."`

2. SharePlugin (`ratatoskr/shareplugin/main.cpp`):
   - Add `qDebug()` statement immediately after `QGuiApplication` initialization
   - Format: `"Ratatoskr SharePlugin" BUILD_VERSION "starting..."`

3. Version generation is handled entirely by CMake inside the clickable
   build container, using `git describe --tags` against the mounted
   git repository.

4. Build process: run `clickable build` (or `clickable build --arch arm64`)
   directly from the `ratatoskr/` subfolder. No wrapper script is needed.

## Amendment (2026-03-27) — Issue #24

The original implementation used a `build.sh` wrapper and `get-version.sh`
script to work around git tags not being visible inside the clickable Docker
container. Root cause analysis (Issue #24) revealed that clickable only
mounts the directory containing `clickable.yaml`, not the full git
repository root. Since `.git` lives one level above `clickable.yaml`,
`git describe` failed inside the container.

**Fix applied:**
- Added `root_dir: ..` to `clickable.yaml` so the entire git repo root
  (including `.git`) is mounted into the container
- Added `src_dir: ${ROOT}/ratatoskr` to direct CMake to the correct
  source directory
- Added `prebuild: git config --global --add safe.directory ${ROOT}` to
  handle the git ownership check (container runs as root, files owned by
  host user)

**Removed:**
- `build.sh` wrapper script
- `get-version.sh` host-side version generator
- `VERSION.txt` intermediate file and its `.gitignore` entry

## Consequences

### Positive
- Single source of truth (git tags)
- Automatic version generation
- No manual version maintenance in code
- Consistent versioning across both executables
- Easy to identify build in logs
- Standard `clickable build` workflow — no custom wrapper needed

### Negative
- Requires git repository for builds (already a requirement)
- Version only known at compile time (not modifiable at runtime)
- Timestamp-based dev versions not semantic versioning compliant

### Neutral
- Developers must use git tags for releases
- Version information appears in logs (requires log access for support)

## References
- Root CMakeLists.txt lines 45-63 (version generation)
- Sprint #010 scope: "App and SharePlugin log their version at startup"
- Issue #24: Get rid of 'build.sh' wrapper
