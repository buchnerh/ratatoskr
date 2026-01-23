# SharePlugin ContentHub Fix - Sprint #007 (Update 2)

**Date:** 2026-01-22  
**Issue:** SharePlugin starts but no file transfer occurs

## Problem Analysis

From the test report and logs, the SharePlugin successfully:
1. ✅ Starts without crashing
2. ✅ Discovers Bluetooth devices (including laptop at 94:E6:F7:22:D6:56)
3. ✅ Receives file path from ContentHub: `/home/phablet/.cache/ratatoskr.philipa/HubIncoming/3/Adrien_Roge.vcf`
4. ❌ But clicking on a device does nothing - transfer never initiates

## Root Cause

**Variable naming collision in Main.qml:**

The QML code used `transfer` for **two different things**:

```qml
BtTransfer {
    id: transfer  // ← Bluetooth transfer object
}

Connections {
    target: ContentHub
    onShareRequested: {
        for (var i = 0; i < transfer.items.length; i++) {  // ← ContentHub transfer parameter
            var filePath = String(transfer.items[i].url).replace('file://', '')
            tmp.push(filePath);
        }
    }
}
```

The handler tried to access `transfer.items` (expecting ContentHub's transfer), but `transfer` was actually the `BtTransfer` object which has no `items` property. This caused:
- No files added to `root.fileNames` array
- Clicking device executed `for` loop with empty array
- No actual Bluetooth transfer initiated

## Solution Applied

**Renamed BtTransfer instance to `btTransfer` to avoid collision:**

```qml
BtTransfer {
    id: btTransfer  // ← Now distinct from ContentHub's 'transfer' parameter
}
```

Updated all references throughout Main.qml:
- `btTransfer.sendFile()` - Initiate transfer
- `btTransfer.currentFile` - Track progress
- `btTransfer.progress` - Progress bar value
- `btTransfer.finished` - Transfer completion
- `btTransfer.error` - Error status

The `onShareRequested` handler now correctly accesses:
- `transfer.items` - ContentHub transfer parameter (passed automatically)
- `btTransfer.sendFile()` - Bluetooth transfer method

## Additional Fix

**Removed obsolete `plugins` directory reference from CMakeLists.txt:**

The directory existed but was empty (only placeholder CMakeLists.txt). Removed:
```cmake
add_subdirectory(plugins)
```

## Changes Made

### 1. shareplugin/Main.qml
- Renamed `BtTransfer` instance from `transfer` to `btTransfer`
- Updated all Bluetooth transfer references
- Added debug logging for ContentHub
- Updated version to 0.0.5

### 2. CMakeLists.txt
- Removed `add_subdirectory(plugins)` line

## Testing Plan

1. **Install updated package** (`ratatoskr.philipa_260122165030_arm64.click`)
2. **Share a contact from Contacts app**
3. **Select "Bluetooth" from share menu**
   - Should display device list
   - Should show the file to be transferred
4. **Click on laptop device** (94:E6:F7:22:D6:56)
   - Should stop device discovery
   - Should show "Transferring..." message
   - Should display progress bar
5. **Verify transfer on laptop**
   - Check if file transfer request appears
   - Accept transfer
   - Verify file received

## Expected Behavior

### Before Fix
```
Share contact → Select Bluetooth → Device list appears → Click device → Nothing happens (black screen stays)
```

### After Fix
```
Share contact → Select Bluetooth → Device list appears → Click device → "Transferring..." appears → Progress shown → Transfer completes
```

## Technical Details

### ContentHub Integration Pattern

ContentHub automatically provides a `transfer` parameter to `onShareRequested`:

```qml
Connections {
    target: ContentHub
    onShareRequested: {
        // 'transfer' parameter is provided by ContentHub
        // transfer.items is an array of ContentItem objects
        for (var i = 0; i < transfer.items.length; i++) {
            var url = transfer.items[i].url  // file:// URL
        }
    }
}
```

This is standard ContentHub API - the parameter name must remain `transfer`.

### Bluetooth Transfer Flow

1. ContentHub passes file URLs to SharePlugin
2. SharePlugin extracts file paths and stores in `root.fileNames`
3. User selects Bluetooth device from list
4. `btTransfer.sendFile(address, filePath)` called for each file
5. Qt's `QBluetoothTransferManager` handles the actual transfer
6. Progress updates via signals from `BtTransfer`

## New Package Details

- **amd64**: `ratatoskr.philipa_260122164959_amd64.click` (123 KB)
- **arm64**: `ratatoskr.philipa_260122165030_arm64.click` (117 KB)

## Files Modified

```
ratatoskr/shareplugin/Main.qml - ContentHub variable fix
ratatoskr/CMakeLists.txt - Removed plugins directory
```

## References

- Test Report: `docs/sprint-007/SharePlugin_test_report.md`
- Journal Logs: `docs/sprint-007/journal_cropped-20260122-2.txt`
- ContentHub API: Lomiri.Content 1.3 documentation
