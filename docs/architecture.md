# Ratatoskr Architecture Overview

**Version**: 0.0.1  
**Date**: 2025-12-18  
**Based on**: ubtd-20.04 by Michael Zanetti and Ian L.

## Architecture Summary

Ratatoskr consists of two main executables:

1. **Main Application** (`ratatoskr`) - Receives files via Bluetooth
2. **SharePlugin** (`shareplugin`) - Sends files via ContentHub integration

## Component Breakdown

### Main Application Components

#### 1. Main Entry Point
- **File**: `main.cpp`
- **Purpose**: Application initialization and QML engine setup
- **Key Features**:
  - Registers Ratatoskr QML module
  - Initializes OBEX daemon interface
  - Sets up QML context properties

#### 2. OBEX Daemon Interface
- **Files**: `obexd.cpp`, `obexd.h`
- **Purpose**: D-Bus interface to BlueZ OBEX daemon for receiving files
- **Technology**: Qt D-Bus, `org.openobex` service
- **Key Features**:
  - Registers OBEX agent with system
  - Manages incoming transfer requests
  - Provides QAbstractListModel for QML

#### 3. OBEX Agent
- **Files**: `obexagent.cpp`, `obexagent.h`, `obexagentadaptor.cpp`, `obexagentadaptor.h`
- **Purpose**: D-Bus adaptor for OBEX file transfer authorization
- **Key Features**:
  - Authorizes incoming file transfers
  - Provides file paths for received files
  - Emits signals for QML UI updates

#### 4. Transfer Model
- **Files**: `transfer.cpp`, `transfer.h`
- **Purpose**: Tracks individual file transfer progress
- **Key Features**:
  - D-Bus interface to transfer objects
  - Progress monitoring
  - File metadata (name, size, status)

#### 5. Bluetooth Adapter
- **Files**: `adapter.cpp`, `adapter.h`
- **Purpose**: BlueZ adapter management
- **Key Features**:
  - Makes device discoverable
  - Manages Bluetooth adapter state

### SharePlugin Components

#### 1. SharePlugin Entry Point
- **File**: `shareplugin/main.cpp`
- **Purpose**: ContentHub integration for sending files
- **Key Features**:
  - Registers BtTransfer QML type
  - Provides UI for device selection and sending

#### 2. Bluetooth Transfer
- **Files**: `shareplugin/bttransfer.cpp`, `bttransfer.h`
- **Purpose**: Qt Bluetooth file sending via OBEX OPP
- **Technology**: `QBluetoothTransferManager`
- **Key Features**:
  - Queue-based file sending
  - Progress tracking
  - Error handling

## Technology Stack

### Core Technologies
- **Language**: C++  
- **Framework**: Qt 5.12.9
- **UI**: QML with Lomiri Components 1.3
- **Bluetooth**: BlueZ stack

### Qt Modules Used
- Qt Core
- Qt QML
- Qt Quick
- Qt Quick Controls 2
- **Qt DBus** - For OBEX daemon communication
- **Qt Bluetooth** - For OBEX file sending

### D-Bus Services
- **org.bluez** - Bluetooth adapter management
- **org.openobex** - OBEX daemon for receiving files

## Communication Flow

### Receiving Files
```
Remote Device → BlueZ → bluez-obexd (D-Bus) → ObexAgent → Obexd → QML UI
```

1. Remote device initiates OBEX push
2. BlueZ handles Bluetooth connection
3. bluez-obexd daemon manages OBEX protocol
4. ObexAgent (via D-Bus) authorizes transfer
5. Obexd tracks transfer progress
6. QML UI displays status

### Sending Files
```
QML UI → BtTransfer → QBluetoothTransferManager → BlueZ → Remote Device
```

1. User selects file in QML UI
2. BtTransfer queues file for sending
3. QBluetoothTransferManager handles OBEX OPP
4. BlueZ transmits file over Bluetooth
5. Progress updates to QML UI

## File Organization

```
ratatoskr/
├── main.cpp                    # Main app entry point
├── Main.qml                    # Main UI
├── adapter.cpp/h               # Bluetooth adapter management
├── obexd.cpp/h                 # OBEX daemon D-Bus interface
├── obexagent.cpp/h             # OBEX agent implementation
├── obexagentadaptor.cpp/h      # OBEX agent D-Bus adaptor
├── transfer.cpp/h              # Transfer model
├── CMakeLists.txt              # Build configuration
├── manifest.json.in            # App manifest
├── ratatoskr.apparmor          # AppArmor security profile
├── ratatoskr.desktop.in        # Desktop entry
└── shareplugin/                # ContentHub plugin
    ├── main.cpp                # SharePlugin entry
    ├── Main.qml                # Send UI
    ├── bttransfer.cpp/h        # Bluetooth sending
    ├── adapter.cpp/h           # Adapter (duplicate)
    ├── CMakeLists.txt          # Plugin build config
    ├── shareplugin.apparmor    # Plugin AppArmor profile
    ├── shareplugin.desktop     # Plugin desktop entry
    └── bluetooth-contenthub.json  # ContentHub config
```

## Security

### AppArmor Policies
- **Template**: `unconfined` (required for Bluetooth access)
- **Policy Groups**:
  - `networking` - Network access
  - `content_exchange_source` - ContentHub integration
- **Policy Version**: 20.04

### Permissions Required
- Bluetooth device access
- D-Bus system bus access (for BlueZ)
- D-Bus session bus access (for OBEX daemon)
- File system access for saving/loading files

## Known Limitations (Sprint 001)

1. **QML Component Issues**: UbuntuShape deprecated in Lomiri, needs update
2. **No Bluetooth Hardware in Desktop Mode**: OBEX daemon not available in emulator
3. **File Picker**: Not yet integrated
4. **Contacts Export**: VCF generation not yet implemented

## Future Enhancements

### Sprint 002+
- Fix QML component deprecations
- Integrate contacts API for VCF export
- Enhanced device discovery UI
- Car kit compatibility testing
- Better error handling and user feedback

## References

- Original work: ubtd by Michael Zanetti (Canonical)
- UT 16.04/20.04 ports: Ian L. (floop2/fourloop2002)
- Qt Bluetooth: https://doc.qt.io/qt-5/qtbluetooth-index.html
- BlueZ: http://www.bluez.org/
- OBEX Protocol: IrDA Object Exchange Protocol
