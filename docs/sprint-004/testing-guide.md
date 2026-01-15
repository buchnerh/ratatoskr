# Quick Testing Guide: OBEX Service Discovery Fix

**Purpose**: Verify the OBEX service discovery and agent registration fix works on device.

## Prerequisites

- Ubuntu Touch device with Bluetooth enabled
- PC paired with device via Bluetooth
- SSH access to device (for logs)
- Latest build installed: `ratatoskr.philipa_260115151846_amd64.click`

## Test Scenario 1: Service Not Running

**Goal**: Verify graceful handling when `obexd` is not running.

### Steps:
1. Stop obexd if running:
   ```bash
   systemctl --user stop obex.service
   ```

2. Launch ratatoskr app

3. Check logs:
   ```bash
   journalctl --user -f | grep -E "(ratatoskr|obex)"
   ```

### Expected Result:
- App starts successfully
- Log shows:
  ```
  discovering obex service
  Named service not found, attempting D-Bus activation
  OBEX service not found. File transfers will not work.
  Try starting obexd manually: systemctl --user start obex.service
  ```
- No crash or hang

## Test Scenario 2: Service Running

**Goal**: Verify service discovery and agent registration work.

### Steps:
1. Start obexd:
   ```bash
   systemctl --user start obex.service
   ```

2. Verify it's running:
   ```bash
   systemctl --user status obex.service
   busctl --user list | grep -E "(bluez|obex)"
   ```

3. Close and restart ratatoskr app

4. Check logs:
   ```bash
   journalctl --user -f | grep -E "(ratatoskr|obex)"
   ```

### Expected Result:
- Log shows:
  ```
  discovering obex service
  Found OBEX service by name: org.bluez.obex
  found OBEX service at: org.bluez.obex
  registering agent on obexd-server
  Agent registered successfully
  ```
- No error messages about service not found

## Test Scenario 3: File Transfer (The Main Test!)

**Goal**: Verify files can be transferred from PC to phone.

### Steps:
1. Ensure obexd is running (from Test 2)
2. Ensure ratatoskr app is running
3. From PC: Send a small test file via Bluetooth to phone
4. Watch both:
   - Phone screen (ratatoskr app)
   - Logs: `journalctl --user -f`

### Expected Result:
- File appears in ratatoskr "reception area"
- Log shows:
  ```
  CONNECT(0x0), Success(0x20)
  PUT(0x2), Success(0x20)
  authorize called /org/bluez/obex/...
  new transfer /org/bluez/obex/...
  added to model
  ```
- **NO "Forbidden (0x43)" error**
- File saved to cache location

### Success Indicators:
✅ Transfer completes on PC side  
✅ File appears in ratatoskr UI  
✅ File exists in `/home/phablet/.cache/ratatoskr.philipa/`  
✅ No AppArmor denials in logs

## Test Scenario 4: D-Bus Activation

**Goal**: Test if D-Bus can auto-start obexd.

### Steps:
1. Stop obexd:
   ```bash
   systemctl --user stop obex.service
   ```

2. Launch ratatoskr app

3. Immediately check if obexd auto-started:
   ```bash
   systemctl --user status obex.service
   ```

### Expected Result:
- If D-Bus activation works: obexd should be "active (running)"
- If it doesn't work: obexd remains "inactive (dead)"
- Either way, app should handle it gracefully

## Quick Diagnostic Commands

### Check Service Status:
```bash
# Session bus services
busctl --user list | grep -E "(bluez|obex)"

# OBEX daemon status
systemctl --user status obex.service

# Check if service name is registered
busctl --user call org.freedesktop.DBus /org/freedesktop/DBus \
  org.freedesktop.DBus NameHasOwner s "org.bluez.obex"
```

### Check Recent Logs:
```bash
# App logs
journalctl --user -u lomiri-app-launch* | grep ratatoskr | tail -50

# OBEX daemon logs
journalctl --user -u obex.service | tail -50

# Combined relevant logs
journalctl --user -n 100 | grep -E "(ratatoskr|obex|bluez)"
```

### Check File Transfers:
```bash
# Cached files
ls -lah ~/.cache/ratatoskr.philipa/

# D-Bus objects (while transfer in progress)
busctl --user tree org.bluez.obex
```

## Common Issues & Solutions

### Issue: "Service not found" even though obexd is running

**Check**:
```bash
busctl --user list | grep obex
```

**Solution**: The service might be at a dynamic address like `:1.129`. The app should find it automatically, but check logs for "Found OBEX service at dynamic address".

### Issue: "Agent registration failed"

**Check AppArmor denials**:
```bash
sudo journalctl -n 100 | grep "apparmor.*DENIED.*ratatoskr"
```

**Solution**: May need AppArmor policy adjustment for D-Bus method calls.

### Issue: Transfer shows success on PC but no file on phone

**Check**:
1. Agent authorization: `grep "authorize called" ~/.cache/upstart/...`
2. Transfer object creation: `grep "new transfer" ~/.cache/upstart/...`
3. File write permissions: `ls -ld ~/.cache/ratatoskr.philipa/`

## Success Checklist

After running all tests, verify:

- [ ] App starts when obexd not running (graceful degradation)
- [ ] App discovers obexd when it is running
- [ ] Agent registration succeeds
- [ ] File transfers from PC show in app UI
- [ ] Files are saved to cache directory
- [ ] No "Forbidden (0x43)" errors
- [ ] No system crashes or restarts
- [ ] Logs show clear diagnostic information

## Report Format

When reporting results, please include:

1. **Test scenario** that was run
2. **Actual behavior** observed
3. **Log excerpts** (especially any errors)
4. **Screenshots** if UI issues
5. **Files created** in cache directory (if any)

Example:
```
Test: File Transfer (Scenario 3)
Result: ✅ Success
Notes: File appeared after 2 seconds, saved to cache
Log: "Agent registered successfully", no errors
```

## Build to Test

- Package: `ratatoskr.philipa_260115151846_amd64.click`
- Build date: 2026-01-15
- Changes: Session bus + service discovery + retry logic
