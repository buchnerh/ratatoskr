# Sprint #003 - Quick Reference

**Branch:** sprint-003  
**Version:** 0.0.3  
**Status:** Ready for Implementation  
**Estimated Effort:** 15-23 hours

## The Four Issues

| # | Issue | Severity | Fix Complexity | Time |
|---|-------|----------|----------------|------|
| 1 | AppArmor Bluetooth Access Denied | Critical | Low | 1-2h |
| 2 | SharePlugin System Crash | Critical | Medium-High | 6-9h |
| 3 | SharePlugin Icon Missing | High | Trivial | 15m |
| 4 | Bluetooth Discoverability | Medium | Unknown | 2-3h |

## Implementation Order

1. **Icon Fix** (15 min) - Quick win, improves UX immediately
2. **AppArmor Fix** (1-2 hours) - Unblocks core Bluetooth functionality
3. **SharePlugin Crash Fix** (6-9 hours) - Most complex, most critical
4. **Discoverability Investigation** (2-3 hours) - May not need fixing

## Key Files to Modify

### Task 1: Icon
- `ratatoskr/shareplugin/shareplugin.desktop`

### Task 2: AppArmor
- `ratatoskr/obexd.cpp` (line 22)

### Task 3: SharePlugin Crash
- `ratatoskr/shareplugin/main.cpp` (switch engine)
- `ratatoskr/shareplugin/Main.qml` (add cleanup, fix timer)
- `ratatoskr/shareplugin/bttransfer.h` (declare destructor)
- `ratatoskr/shareplugin/bttransfer.cpp` (implement destructor)

### Task 4: Discoverability
- Investigation first
- Possibly `ratatoskr/obexd.cpp` (add server registration)

## Critical Code Changes

### Icon Fix
```diff
- Icon=assets/logo.svg
+ Icon=ratatoskr
```

### AppArmor Fix
```diff
- m_dbus(QDBusConnection::sessionBus()),
+ m_dbus(QDBusConnection::systemBus()),
```

### SharePlugin Main Change
```diff
- QQuickView view;
- view.setSource(...);
- return app.exec();
+ QQmlApplicationEngine engine;
+ engine.load(...);
+ return app.exec();
```

## Testing Checklist

### After Each Fix
- [ ] Build successful
- [ ] Deploy to device
- [ ] Function test
- [ ] Log check (no errors)
- [ ] System stability check

### Final Integration
- [ ] All 4 tasks completed
- [ ] Main app works end-to-end
- [ ] SharePlugin works without crash
- [ ] Multiple invocation cycles successful
- [ ] Version updated to 0.0.3

## Success Criteria

- Main app registers OBEX agent (no AccessDenied)
- SharePlugin invocation doesn't crash system
- Icon displays in share menu
- No threading errors in logs
- System stable after operations

## Documentation

- **Full Plan:** `docs/sprint-003/sprint-003-plan.md`
- **Research:** `docs/sprint-003/research-findings.md`
- **This File:** `docs/sprint-003/quick-reference.md`

## Commands

```bash
# Build for emulator
clickable build

# Build for device (arm64)
clickable build --arch arm64

# Install on device
clickable install

# View logs on device
adb logcat | grep ratatoskr
```

## Risk Mitigation

- Each fix committed separately
- Test independently before integration
- Keep git history clean for rollback
- Document unexpected behaviors immediately

---

**Ready to start? Begin with Task 1 (Icon Fix) - the quick win!**
