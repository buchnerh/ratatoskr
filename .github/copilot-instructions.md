# Ratatoskr - Bluetooth File Transfer for Ubuntu Touch

A Qt/QML-based Bluetooth file transfer application for Ubuntu Touch 20.04, optimized for sending contacts (VCF cards) to car kits and bi-directional file transfer with PCs.

## Build Commands

Build and run commands use `clickable`, which manages Docker containers with the compiler toolchain:

```bash
# Build for emulator (host architecture)
cd ratatoskr/
clickable build

# Build for device (aarch64)
cd ratatoskr/
clickable build --arch arm64

# Run in desktop mode (emulator)
clickable desktop
```

**Note**: No test suite currently exists in this project.

## Architecture Overview

Ratatoskr consists of **two executables**:

### 1. Main Application (`ratatoskr`)
**Purpose**: Receives files via Bluetooth using OBEX protocol

**Key Components**:
- **ObexAgent** (`obexagent.cpp/h`, `obexagentadaptor.cpp/h`): D-Bus adaptor that registers with `org.openobex` service to authorize incoming file transfers
- **Obexd** (`obexd.cpp/h`): Main interface to BlueZ OBEX daemon, manages transfer tracking via D-Bus
- **Transfer** (`transfer.cpp/h`): Models individual file transfers with progress tracking
- **Adapter** (`adapter.cpp/h`): Manages Bluetooth adapter state and device discoverability
- **BluetoothManager** (`bluetoothmanager.cpp/h`): Higher-level Bluetooth operations
- **DeviceDiscovery** (`devicediscovery.cpp/h`): Device scanning and discovery

**Communication Flow**: `Remote Device → BlueZ → bluez-obexd (D-Bus) → ObexAgent → Obexd → QML UI`

### 2. SharePlugin (`shareplugin`)
**Purpose**: Sends files via ContentHub integration

**Key Components**:
- **BtTransfer** (`bttransfer.cpp/h`): Uses `QBluetoothTransferManager` for OBEX OPP file sending
- **DeviceNameResolver** (`devicenameresolver.cpp/h`): Resolves Bluetooth device names
- Separate `adapter.cpp/h` (duplicate from main app)

**Communication Flow**: `ContentHub → QML UI → BtTransfer → QBluetoothTransferManager → BlueZ → Remote Device`

## Code Style & File Headers

- **Indentation**: 2 spaces (not tabs)
- **File Headers**: 
  - Use template from `templates/header.cpp` for all C++/h/qml files inherited from 'ubtd'
  - Use template from `templates/header_2.cpp` for all new C++/h/qml files
- **Header Updates**: When modifying any source file:
  - Update the "Date" field to modification date
  - Update the "Version" field
  - Add entry to "Modification History" section

Example header format:
```cpp
/*==========================================================
 * Program : filename.cpp                Project : ratatoskr
 * Author  : Michael Zanetti, Ian L., Philippe Andersson
 * Date    : 2026-01-15
 * Version : 0.0.2
 * Notice  : (c) Original work by Michael Zanetti, Canonical
 *           Adapted by Ian L. and Philippe Andersson
 * License : GNU GPL v3 or later
 * Comment : Brief description of file content.
 *           Based on 'ubtd' by Michael Zanetti and Ian L.
 *           Revived with the help of the Copilot CLI coding agent.
 * Modification History:
 * - 2025-12-18 (0.0.1) : Initial release.
 * - 2026-01-15 (0.0.2) : Added feature X.
 *========================================================*/
```

## Technology Stack

- **Language**: C++ (GNU C++ 9.4.0)
- **Framework**: Qt 5.12.9 with QML
- **UI Components**: Lomiri Components 1.3 (Ubuntu Touch)
- **Bluetooth**: BlueZ stack via Qt Bluetooth and Qt DBus
- **Build System**: CMake 3.0+ with Qt automoc
- **Container**: Docker-based via clickable-ut 8.6.0

### Qt Modules Used
- Qt Core, Qml, Quick, QuickControls2
- **Qt DBus**: Critical for OBEX daemon communication (receiving files)
- **Qt Bluetooth**: Critical for OBEX OPP (sending files)

### D-Bus Services
- `org.bluez`: Bluetooth adapter management
- `org.openobex`: OBEX daemon (bluez-obexd) for receiving files

## Security & Permissions

**AppArmor Profile**:
- Template: `unconfined` (required for Bluetooth hardware access)
- Policy groups: `networking`, `content_exchange_source`
- Policy version: 20.04
- Files: `ratatoskr.apparmor`, `shareplugin/shareplugin.apparmor`

**Critical Permissions**:
- D-Bus system bus (BlueZ communication)
- D-Bus session bus (OBEX daemon)
- Bluetooth device access
- Filesystem access for received/sent files

## Project Management

### Documentation Structure
- `docs/`: Technical documentation and ADRs
- ADRs follow naming: `ADR-XXX-descriptive-title.md`
- ADR index maintained in `CLAUDE.md`

### Development Process
- Work in sprints with dedicated branches: `sprint-XXX` (e.g., `sprint-001`, `sprint-002`) during inception
- Work in branches created from Github issues after first public release
- **DO NOT** merge sprint / issue branches to `master` - maintainer handles merges
- Semantic versioning: Currently targeting `0.0.1` (project inception)
- Sprint documentation stored in `docs/sprint-XXX/` directories
- all commits by Copilot feature a suitable `Co-developed-by:` git trailer

### Important ADRs
- ADR-001: Bluetooth architecture strategy (based on ubtd-20.04)
- ADR-002: AppArmor security policy for Bluetooth access
- ADR-003: OBEX D-Bus service discovery and agent registration
- ADR-004: AppArmor D-Bus receive permissions for OBEX agent
- ADR-005: SharePlugin NoDisplay issue and ContentHub integration

## Key Conventions

### Dual-Executable Pattern
The project splits functionality into two executables for security/architectural reasons:
- Main app: Passive receiver (always running, registered OBEX agent)
- SharePlugin: Active sender (launched via ContentHub)

This is intentional and both apps share some code (e.g., `adapter.cpp/h` duplicated in `shareplugin/`).

### D-Bus Adaptor Generation
`obexagentadaptor.cpp/h` are manually maintained D-Bus adaptors (not auto-generated). When modifying OBEX agent interface, both the agent and adaptor must be updated in sync.

### CMake Build Version
`CMakeLists.txt` auto-generates version from git tags:
- Tagged commits use tag as version
- Untagged commits use `{last-tag}.{timestamp}`
- Version exposed via `-DBUILD_VERSION` define

### ContentHub Integration
SharePlugin uses `bluetooth-contenthub.json` to register as a content sharing destination. The desktop file must have `NoDisplay=true` set (see ADR-005).

## Known Limitations

- No Bluetooth hardware available in desktop/emulator mode (bluez-obexd not available)
- VCF generation from contacts API not yet implemented
- Some QML components (UbuntuShape) deprecated in Lomiri, pending updates

## Attribution

Based on 'ubtd' (Bluetooth File Transfer) originally developed by Michael Zanetti (Canonical) and ported to Ubuntu Touch 16.04/20.04 by Ian L. (floop2/fourloop2002).
