# Sprint #003 Plan

**Target Version:** 0.0.3  
**Created:** 2025-12-25  
**Status:** Draft - Pending research

## Executive Summary

Sprint #003 focuses on resolving critical blocking issues discovered during Sprint #002 testing. While the application architecture and UI implementation from Sprint #002 are solid, several runtime issues prevent the app from functioning on real hardware.

## Issues from Sprint #002

### Critical Issues (Blocking)

1. **ISSUE-001: AppArmor Bluetooth Access Denied**
   - **Severity:** Critical
   - **Description:** Main app cannot register OBEX agent due to AppArmor policy denial
   - **Evidence:** `org.freedesktop.DBus.Error.AccessDenied` in main app logs
   - **Impact:** Core Bluetooth functionality completely blocked
   - **Root Cause:** TBD (requires research into Ubuntu Touch AppArmor requirements)

2. **ISSUE-002: SharePlugin System Crash**
   - **Severity:** Critical
   - **Description:** Phone instantly reboots when SharePlugin is invoked from Contacts app
   - **Evidence:** 
     - `QObject::~QObject: Timers cannot be stopped from another thread`
     - `lost connection to Mir server`
   - **Impact:** SharePlugin completely unusable; potential data loss risk for users
   - **Root Cause:** Threading/lifecycle management issue in SharePlugin implementation

3. **ISSUE-003: SharePlugin Icon Not Displayed**
   - **Severity:** High
   - **Description:** SharePlugin shows blank icon in content-hub share menu
   - **Evidence:** `Failed to get image from provider: image://content-hub/ratatoskr.philipa_shareplugin_251220171217`
   - **Impact:** Poor user experience; app not easily identifiable in share menu
   - **Root Cause:** Incorrect icon path in shareplugin.desktop file

### High Priority Issues (Non-Blocking)

4. **ISSUE-004: Bluetooth Discoverability**
   - **Severity:** High
   - **Description:** Phone not discoverable to other Bluetooth devices (laptop test)
   - **Impact:** Cannot establish new pairings while app is intended to run
   - **Root Cause:** TBD (may be OBEX server profile not registered, or system-level issue)

## Sprint #003 Objectives

### Primary Goals

1. **Fix AppArmor Policy** (ISSUE-001)
   - Research Ubuntu Touch AppArmor requirements for Bluetooth access
   - Identify proper policy configuration or alternative approaches
   - Test and verify Bluetooth stack access on real hardware

2. **Fix SharePlugin Crash** (ISSUE-002)
   - Debug threading model in SharePlugin implementation
   - Review QObject lifecycle management
   - Implement proper thread-safe cleanup
   - Test SharePlugin invocation without system crash

3. **Fix SharePlugin Icon** (ISSUE-003)
   - Correct icon path in shareplugin.desktop
   - Test icon display in content-hub share menu

### Secondary Goals

4. **Investigate Bluetooth Discoverability** (ISSUE-004)
   - Determine if issue is app-related or system-level
   - Implement OBEX server profile registration if needed
   - Document findings and resolution approach

## Research Tasks (Pre-Implementation)

Before detailed technical planning, the following research is required:

1. **AppArmor Research**
   - Review Ubuntu Touch AppArmor documentation
   - Examine working Bluetooth apps on OpenStore
   - Identify if confined apps can access Bluetooth at all
   - Document alternative approaches if standard policy groups insufficient

2. **SharePlugin Threading Analysis**
   - Review current SharePlugin implementation architecture
   - Identify threading model and QObject ownership
   - Research content-hub plugin lifecycle requirements
   - Document proper implementation pattern

3. **Content-Hub Icon Path Requirements**
   - Research correct icon path format for content-hub
   - Identify if icon needs to be in specific location
   - Review working examples from other apps

## Success Criteria

- [ ] Main app successfully registers OBEX agent on real hardware
- [ ] SharePlugin can be invoked without causing system crash
- [ ] SharePlugin icon displays correctly in share menu
- [ ] Bluetooth discoverability issue understood and documented (resolution if feasible)
- [ ] All fixes tested on real hardware (not just emulator)

## Out of Scope

- New features
- UI/UX improvements
- Additional file type support
- Performance optimizations
- Translation updates (unless required by fixes)

## Dependencies

- Access to real Ubuntu Touch device for testing
- Ubuntu Touch AppArmor documentation
- Content-Hub plugin development documentation

## Notes

- This sprint is entirely focused on bug fixes from Sprint #002
- No new functionality will be added
- Testing must be performed on real hardware, as emulator doesn't catch these issues
- Sprint #002 provided solid foundation; Sprint #003 makes it actually work

## Next Steps

1. **PENDING:** Merge sprint-002 branch to master
2. **PENDING:** Conduct research tasks listed above
3. **PENDING:** Update this plan with detailed technical approach
4. **PENDING:** Create sprint-003 branch
5. **PENDING:** Begin implementation
