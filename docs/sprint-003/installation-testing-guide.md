# Sprint #003 - Installation & Testing Guide

**Version:** 0.0.3  
**Package:** `ratatoskr.philipa_251225152352_arm64.click`  
**Date:** 2025-12-25

## Quick Installation

### Option 1: Direct Install (Recommended if clickable works)

```bash
cd ratatoskr
clickable install
```

### Option 2: Manual Install (If clickable can't detect device)

1. **Copy package to device:**
   ```bash
   adb push ./ratatoskr/build/aarch64-linux-gnu/app/ratatoskr.philipa_251225152352_arm64.click /home/phablet/Downloads/
   ```

2. **On device:**
   - Open file manager
   - Navigate to Downloads
   - Tap the `.click` file
   - Select "Install with other application"
   - Select "OpenStore"
   - Accept disclaimer about reserved policy
   - Wait for installation to complete

3. **Verify installation:**
   - Check app drawer for "Ratatoskr" icon
   - Check OpenStore → My Apps → Installed

---

## Testing Procedure

### Test 1: Main App - AppArmor Fix

**Purpose:** Verify OBEX agent registration succeeds without AppArmor denial

**Steps:**
1. Open terminal on device or connect via adb
2. Start log monitoring:
   ```bash
   adb logcat | grep ratatoskr
   ```
3. Launch Ratatoskr app
4. Look for these log lines:
   ```
   creating agent on dbus
   registering agent
   registering agent on obexd-server
   ```
5. **Critical:** Check for NO "AccessDenied" error

**Expected Result:** ✅
- Agent registers successfully
- NO `org.freedesktop.DBus.Error.AccessDenied` error
- App shows paired devices list

**If Failed:** ❌
- Log shows AccessDenied → system bus fix didn't work
- Document exact error message
- May need to investigate obexd configuration

---

### Test 2: SharePlugin - Icon Display

**Purpose:** Verify icon appears in share menu

**Steps:**
1. Open Contacts app
2. Select any contact
3. Tap share icon (top-right corner)
4. Look at share options list

**Expected Result:** ✅
- "Bluetooth" option appears with Ratatoskr icon
- Icon is visible (not blank square)

**If Failed:** ❌
- Blank icon → ContentHub not resolving icon path
- No "Bluetooth" option → ContentHub registration failed

---

### Test 3: SharePlugin - Crash Prevention

**Purpose:** Verify plugin doesn't cause system crash

**Steps:**
1. Open Contacts app
2. Select any contact
3. Tap share icon
4. Tap "Bluetooth" option
5. SharePlugin window appears
6. **Test A:** Cancel immediately (back button or close)
7. **Observe:** System should remain stable
8. **Test B:** Select a paired device, let transfer attempt
9. **Test C:** Invoke again from another contact
10. Repeat 3-5 times

**Expected Result:** ✅
- Plugin opens and closes cleanly
- No system reboot
- No system freeze
- Can invoke multiple times
- Logs show no threading errors

**If Failed:** ❌
- System reboots → lifecycle fix incomplete
- Check logs for: `QObject::~QObject: Timers cannot be stopped`
- Check logs for: `lost connection to Mir server`

---

### Test 4: SharePlugin - Comprehensive Testing

**Purpose:** Full functional test of SharePlugin

**Steps:**
1. Ensure Bluetooth is enabled on device
2. Ensure at least one device is paired
3. Open Contacts app
4. Select contact
5. Share → Bluetooth
6. Select paired device from list
7. Wait for transfer to complete or fail
8. Check logs during transfer

**Expected Result:** ✅
- Device list appears
- Selection works
- Transfer initiates (may fail if device not accepting)
- No crash during transfer
- Plugin closes cleanly after transfer

**If Failed:** ❌
- Empty device list → Bluetooth discovery issue
- Crash during transfer → BtTransfer destructor issue
- Transfer never starts → OBEX client issue

---

### Test 5: Bluetooth Discoverability (Investigation)

**Purpose:** Determine if discoverability issue is app-related

**Steps:**
1. **System-level test:**
   - On device: Settings → Bluetooth → Enable "Visible to nearby devices"
   - On laptop: Enable Bluetooth, start scanning
   - Try to pair laptop ↔ device through system settings
   - Document result

2. **If system pairing works:**
   - Issue is NOT Ratatoskr-related
   - System Bluetooth works fine
   - No app fix needed

3. **If system pairing fails:**
   - Issue is system-level, not app-specific
   - Check Ubuntu Touch Bluetooth system configuration
   - May be phone model limitation
   - Document as known limitation

**Expected Result:** 🔍
- Understanding of whether issue is app-specific or system-level
- Documentation for potential Sprint #004 work

---

## Log Collection

### During Testing

Keep logs running:
```bash
adb logcat > ratatoskr-test-log.txt
```

Or on device:
```bash
journalctl -f | grep ratatoskr > /home/phablet/ratatoskr-test.log
```

### Important Log Lines to Capture

**Good signs:**
- "registering agent on obexd-server"
- "creating agent on dbus"
- "SharePlugin: Cleaning up before quit"
- "Should share file [path]"

**Bad signs:**
- "org.freedesktop.DBus.Error.AccessDenied"
- "QObject::~QObject: Timers cannot be stopped"
- "lost connection to Mir server"
- "Error registering agent"

---

## Test Results Template

Copy this and fill in results:

```markdown
# Sprint #003 Test Results

Date: YYYY-MM-DD
Device: [phone model]
Ubuntu Touch: [version]
Package: ratatoskr.philipa_251225152352_arm64.click

## Test 1: Main App - AppArmor
- [ ] PASS / [ ] FAIL
Notes: 

## Test 2: SharePlugin Icon
- [ ] PASS / [ ] FAIL
Notes:

## Test 3: SharePlugin Crash Prevention
- [ ] PASS / [ ] FAIL
Notes:

## Test 4: SharePlugin Comprehensive
- [ ] PASS / [ ] FAIL
Notes:

## Test 5: Discoverability Investigation
- [ ] System pairing works / [ ] System pairing fails
Notes:

## Overall Assessment
- Critical issues fixed: YES / NO
- Ready for merge: YES / NO / WITH CAVEATS

## Issues Found
1. 
2. 

## Logs Attached
- [ ] Main app logs
- [ ] SharePlugin logs
- [ ] System logs (if crash occurred)
```

---

## Troubleshooting

### "Package installation failed"
- Check if old version is still installed
- Uninstall old version first
- Try manual installation method

### "clickable devices" shows nothing
- Use `adb devices` to verify connection
- Use manual installation method (Option 2)

### App doesn't appear after install
- Check OpenStore → My Apps
- Try restarting device
- Check `/home/phablet/.local/share/applications/` for desktop file

### Logs show nothing
- App may not be starting
- Check app permissions in UT Tweak Tool
- Try running from terminal: `/opt/click.ubuntu.com/ratatoskr.philipa/current/ratatoskr`

---

## Success Criteria

### Minimum (Must Pass)
- [x] Main app launches
- [x] No AppArmor AccessDenied errors
- [x] SharePlugin invocation doesn't crash system
- [x] SharePlugin icon displays

### Desired (Should Pass)
- [x] SharePlugin works multiple times
- [x] No threading errors in logs
- [x] File transfer completes (if device available)

### Optional (Nice to Have)
- [ ] Discoverability issue understood
- [ ] Performance is good
- [ ] No memory leaks

---

## Post-Testing

After completing all tests:

1. Save logs to `docs/sprint-003/test_result/`
2. Create `docs/sprint-003/test-results.md` with findings
3. Take screenshots if issues found
4. If all critical tests pass: ready to merge to master
5. If issues found: create tickets for Sprint #004

---

**Good luck with testing! 🎯**
