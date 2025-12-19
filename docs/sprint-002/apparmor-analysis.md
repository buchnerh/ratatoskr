# AppArmor "Unconfined" Template Error - Analysis and Recommendation

**Date**: 2025-12-18  
**Issue**: Automated review rejects app due to `unconfined` AppArmor template  
**Severity**: High (blocks OpenStore publication)  
**Status**: Requires fix in Sprint 002

## Problem Summary

The clickable automated review process fails with:

```
- security:template_valid:ratatoskr.apparmor
    (NEEDS REVIEW) 'unconfined' not allowed
- security:template_valid:shareplugin.apparmor
    (NEEDS REVIEW) 'unconfined' not allowed
```

## Root Cause

We inherited the `unconfined` AppArmor template from Ian L.'s ubtd-20.04:

```json
{
    "policy_groups": ["networking", "content_exchange_source"],
    "policy_version": 20.04,
    "template": "unconfined"
}
```

**Why Ian L. used "unconfined":**

- Rapid development/testing focus
- Bluetooth historically required extensive permissions
- ubtd was a proof-of-concept, not intended for OpenStore
- UT 16.04→20.04 port prioritized functionality over security refinement

## Impact Assessment

### Current State
- ❌ **Cannot be published to OpenStore** - automatic review rejection
- ❌ **Security vulnerability** - no AppArmor protection
- ❌ **Professional concern** - doesn't meet Ubuntu Touch standards
- ✅ **Builds successfully** - works for development
- ✅ **Runs in emulator** - functional testing possible

### Business Impact
- Blocks distribution to users
- Limits testing to sideloading only
- May affect user trust/confidence

## Solution Options Analysis

### ✅ Option 1: Use "bluetooth" Policy Group (RECOMMENDED)

**Configuration:**

```json
{
    "policy_groups": [
        "bluetooth",
        "networking",
        "content_exchange_source"
    ],
    "policy_version": 20.04
}
```

**Assessment:**

- ✅ **Standard approach** for Ubuntu Touch Bluetooth apps
- ✅ **Simple implementation** - just add "bluetooth" to policy groups
- ✅ **Proven functional** - provides necessary BlueZ 5 access
- ✅ **Security maintained** - confined but with Bluetooth permissions
- ⚠️ **Requires manual review** - "bluetooth" is a reserved policy group
- ✅ **Time-efficient** - minimal testing needed

**Risk Level:** **LOW**

**Recommendation:** **Implement this in Sprint 002**

### ⚠️ Option 2: Custom AppArmor Profile

**What it involves:**

- Write custom profile specifying exact D-Bus paths
- Allow specific BlueZ and OBEX daemon access
- Define precise file system paths
- Iterate using complain mode → enforce mode

**Assessment:**

- ✅ **Most secure** - principle of least privilege
- ✅ **Best practice** for security-conscious apps
- ❌ **Time-consuming** - requires AppArmor expertise
- ❌ **Complex testing** - needs iterative refinement
- ❌ **Maintenance burden** - may break with system updates
- ❌ **Out of scope** for current sprint

**Risk Level:** **MEDIUM**

**Recommendation:** **Consider for Sprint 003+ if needed**

### ❌ Option 3: Keep "unconfined" (Development Only)

**Assessment:**

- ✅ **No code changes** 
- ❌ **Cannot publish** - blocks OpenStore entirely
- ❌ **Security risk** - unacceptable for any release
- ❌ **Not professional** - doesn't meet platform standards

**Risk Level:** **HIGH**

**Recommendation:** **DO NOT USE for any release**

## Recommended Action Plan

### Immediate Fix (Sprint 002)

**Step 1: Update AppArmor Configurations**

Edit `ratatoskr/ratatoskr.apparmor`:

```json
{
    "policy_groups": [
        "bluetooth",
        "networking",
        "content_exchange_source"
    ],
    "policy_version": 20.04
}
```

Edit `ratatoskr/shareplugin/shareplugin.apparmor`:

```json
{
    "policy_groups": [
        "bluetooth",
        "networking",
        "content_exchange"
    ],
    "policy_version": 20.04
}
```

**Step 2: Verify Build**

```bash
clickable build
```
Expected: No security template errors in review

**Step 3: Test Functionality**

- Deploy to real Ubuntu Touch device
- Test Bluetooth adapter detection
- Test file sending (OBEX OPP)
- Test file receiving (OBEX agent)
- Monitor logs for AppArmor denials

**Step 4: Document Changes**

- Update Sprint 002 documentation
- Note AppArmor policy change
- Document testing results

### Testing Checklist

After applying the fix, verify:

- [ ] Build completes without review errors
- [ ] App installs on real device
- [ ] Bluetooth adapter is detected
- [ ] Can make device discoverable
- [ ] Can send files via Bluetooth
- [ ] Can receive files via Bluetooth
- [ ] No AppArmor denials in system logs
- [ ] ContentHub integration still works

### Monitoring for Issues

Check system logs for AppArmor denials:

```bash
dmesg | grep DENIED
journalctl | grep apparmor
```

If denials occur, document them for potential custom profile work in future sprints.

## Long-Term Considerations

### OpenStore Publication
- **"bluetooth" policy group** is marked as "reserved"
- Requires **manual review** by OpenStore team
- Provide clear justification: "Bluetooth file transfer app requires BlueZ access"
- Should be approved as this is the standard for BT apps

### Future Security Hardening (Optional)
If we pursue maximum security in Sprint 003+:

1. Profile the app's actual D-Bus calls
2. Create minimal custom AppArmor profile
3. Test in complain mode first
4. Refine based on logs
5. Switch to enforce mode
6. Document all permissions

This is **not required** but could be a differentiator if security is a selling point.

## Decision

**For Sprint 002: Implement Option 1 (bluetooth policy group)**

**Rationale:**

1. Standard, proven approach for Ubuntu Touch Bluetooth apps
2. Minimal implementation effort
3. Allows OpenStore publication (with manual review)
4. Maintains acceptable security posture
5. Enables testing on real hardware
6. Unblocks project progress

**Estimated Effort:** 30 minutes (change files + test build)

## References

- [UBports AppArmor Policy Groups](https://docs.ubports.com/en/latest/appdev/platform/apparmor.html)
- [Ubuntu AppArmor Wiki](https://wiki.ubuntu.com/AppArmor)
- [OpenStore App Submission Guidelines](https://open-store.io/)
- Original ubtd source: github.com/floop2002/ubtd-20.04

## Conclusion

The "unconfined" template was appropriate for Ian L.'s development/proof-of-concept, but not suitable for our production-ready Ratatoskr app. 

**Action: Replace with "bluetooth" policy group in Sprint 002 as first priority.**

This is a straightforward fix that unblocks distribution and maintains security compliance.
