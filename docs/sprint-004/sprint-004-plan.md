# Sprint #004 Plan - Research Sprint

**Target Version:** 0.0.3 (unchanged - research phase)  
**Created:** 2025-12-25  
**Type:** Research & Investigation Sprint  
**Status:** Planning

## Executive Summary

Sprint #004 is a **research-focused sprint** to investigate critical architectural issues discovered during Sprint #003 testing. Rather than continuing implementation with flawed assumptions, we pause to gain deep understanding of Ubuntu Touch's Bluetooth ecosystem and ContentHub plugin architecture.

**No code changes planned** - this sprint focuses on knowledge acquisition and architectural decision-making.

## Background

Sprint #003 testing revealed two fundamental issues:

1. **OBEX Daemon Unavailable**
   - Ubuntu Touch doesn't have `org.bluez.obex` service running
   - File transfer protocol layer missing from platform
   - Current architecture assumes OBEX daemon availability

2. **SharePlugin Not Loading Our Code**
   - Lifecycle fixes from Sprint #003 never executed
   - Same threading errors persist
   - Icon fix didn't apply
   - Suggests ContentHub integration misunderstanding

**Root Cause:** We made incorrect assumptions about Ubuntu Touch platform architecture.

## Sprint Goals

### Primary Objective
**Understand Ubuntu Touch Bluetooth and ContentHub architecture well enough to make informed implementation decisions.**

### Specific Goals

1. **OBEX Investigation**
   - Determine if obexd is available on Ubuntu Touch
   - Identify how to start/enable obexd if present
   - Research alternative approaches if obexd unavailable
   - Document findings with evidence

2. **ContentHub Plugin Architecture**
   - Understand how ContentHub invokes plugins
   - Identify correct plugin implementation pattern
   - Determine why our code changes didn't apply
   - Study working examples from OpenStore

3. **Bluetooth File Transfer Alternatives**
   - Research QtBluetooth standalone approach
   - Investigate if other apps use obexd or alternatives
   - Document pros/cons of different approaches
   - Identify feasible paths forward

4. **Architectural Decision**
   - Based on research, choose implementation approach
   - Document decision rationale
   - Update project architecture documentation
   - Plan Sprint #005 implementation strategy

## Research Tasks

### Task 1: OBEX Daemon Investigation

**Objective:** Determine OBEX availability and requirements on Ubuntu Touch

**Activities:**

1. **Check Package Installation**
   ```bash
   # On device
   dpkg -l | grep -i obex
   apt search obex
   ```
   - Document installed packages
   - Identify available versions
   - Note dependencies

2. **Check Service Status**
   ```bash
   systemctl list-units | grep obex
   systemctl status obexd
   dbus-send --system --print-reply --dest=org.freedesktop.DBus \
     /org/freedesktop/DBus org.freedesktop.DBus.ListNames | grep obex
   ```
   - Document running services
   - Check D-Bus registration
   - Identify why service not available

3. **Research Service Configuration**
   ```bash
   find /etc /usr -name "*obex*" 2>/dev/null
   cat /usr/share/dbus-1/services/*obex*
   cat /lib/systemd/system/*obex*
   ```
   - Locate configuration files
   - Identify service activation methods
   - Document startup requirements

4. **Test Service Startup**
   ```bash
   # If obexd binary exists
   which obexd
   /usr/libexec/bluetooth/obexd --help
   # Try manual start
   /usr/libexec/bluetooth/obexd --systemd --debug
   ```
   - Attempt to start service manually
   - Document errors/requirements
   - Check if AppArmor blocks it

**Deliverables:**
- `obex-investigation.md` - Complete findings report
- Decision: Is obexd viable for our use case?

**Time Estimate:** 4-6 hours

---

### Task 2: ContentHub Plugin Architecture Analysis

**Objective:** Understand correct ContentHub plugin implementation

**Activities:**

1. **Examine Our Installation**
   ```bash
   # On device
   ls -la /opt/click.ubuntu.com/ratatoskr.philipa/current/
   cat /opt/click.ubuntu.com/ratatoskr.philipa/current/manifest.json
   ls -la /home/phablet/.local/share/applications/ratatoskr*
   cat /home/phablet/.local/share/applications/ratatoskr.philipa_shareplugin*.desktop
   ```
   - Verify binary installation
   - Check ContentHub registration
   - Compare with manifest

2. **Test Direct Binary Execution**
   ```bash
   # On device
   /opt/click.ubuntu.com/ratatoskr.philipa/current/shareplugin
   ```
   - Does our binary run?
   - What UI appears?
   - Any errors in logs?

3. **Study Working ContentHub Apps**
   
   **Candidates to analyze:**
   - Dekko2 (email client - has share capability)
   - uNav (navigation - imports/shares locations)
   - Any app with working ContentHub integration
   
   **What to examine:**
   ```bash
   # For each app
   ls -la /opt/click.ubuntu.com/[app-name]/current/
   cat manifest.json
   cat *.desktop
   file [binary-name]  # Is it actually executable?
   ```
   
   - How do they structure their plugins?
   - QML-only vs C++ approach?
   - Desktop file configuration?
   - ContentHub JSON structure?

4. **ContentHub Documentation Review**
   - UBports developer documentation
   - Ubuntu Touch app development guide
   - ContentHub API examples
   - Community forum discussions

5. **Debug ContentHub Invocation**
   ```bash
   # Monitor during SharePlugin invocation
   journalctl -f | grep -i content
   dbus-monitor --session | grep -i content
   ```
   - Watch what ContentHub actually does
   - Identify invocation mechanism
   - Document execution path

**Deliverables:**
- `contenthub-analysis.md` - Architecture findings
- `working-examples.md` - Analysis of successful apps
- Decision: Correct plugin implementation pattern

**Time Estimate:** 6-8 hours

---

### Task 3: Bluetooth File Transfer Alternatives Research

**Objective:** Identify viable approaches for Bluetooth file transfer on Ubuntu Touch

**Activities:**

1. **Qt Bluetooth Capabilities**
   - Review QBluetoothServer documentation
   - Test receiving files via QtBluetooth
   - Identify OBEX protocol support
   - Document limitations

2. **OpenStore Bluetooth Apps**
   
   **Search for:**
   - Bluetooth file transfer apps
   - Apps that receive/send files over Bluetooth
   - Working implementations
   
   **Analyze:**
   - What approach do they use?
   - OBEX, QtBluetooth, or other?
   - Any workarounds for platform limitations?

3. **Ubuntu Touch Forums/Documentation**
   - Search for Bluetooth file transfer discussions
   - Known limitations and workarounds
   - Community solutions
   - Platform roadmap for Bluetooth features

4. **Alternative Protocols**
   - Can we use different approach? (RFCOMM, L2CAP)
   - Trade-offs vs OBEX
   - Device compatibility implications

**Deliverables:**
- `bluetooth-alternatives.md` - Options analysis
- `feasibility-matrix.md` - Compare approaches
- Recommendation for implementation approach

**Time Estimate:** 4-6 hours

---

### Task 4: Device Testing and Validation

**Objective:** Validate findings with hands-on testing

**Activities:**

1. **OBEX Service Experiments**
   - Try to enable obexd (if possible)
   - Test file receiving if service starts
   - Document any success/failures
   - Capture detailed logs

2. **SharePlugin Binary Testing**
   - Run binary directly from terminal
   - Modify and test simple changes
   - Verify code actually executes
   - Identify why ContentHub doesn't use it

3. **QtBluetooth Prototype**
   - Create minimal test app
   - Implement basic file receiving
   - Test on device
   - Document results

4. **ContentHub Minimal Example**
   - Create simplest possible ContentHub plugin
   - Test if it works
   - Gradually add complexity
   - Identify what breaks

**Deliverables:**
- Test results documented
- Proof-of-concept code (if successful)
- Photos/screenshots of testing
- Detailed logs

**Time Estimate:** 6-8 hours

---

## Research Questions to Answer

### Critical Questions (Must Answer)

1. **Is obexd available on Ubuntu Touch?**
   - [ ] Yes, installed by default
   - [ ] Yes, but needs enabling
   - [ ] No, but can be installed
   - [ ] No, and cannot be installed

2. **Can we start obexd service from app?**
   - [ ] Yes, via D-Bus activation
   - [ ] Yes, but requires root
   - [ ] No, system restriction
   - [ ] Unknown, needs testing

3. **Why doesn't ContentHub execute our binary?**
   - [ ] Wrong desktop file configuration
   - [ ] Binary not found/executable
   - [ ] QML-only plugin expected
   - [ ] Other reason: __________

4. **What do working Bluetooth apps use?**
   - [ ] OBEX daemon
   - [ ] QtBluetooth directly
   - [ ] Other approach: __________
   - [ ] No Bluetooth file transfer apps exist

### Important Questions (Should Answer)

5. **Can QtBluetooth replace obexd for our use case?**
6. **What are limitations of each approach?**
7. **Which approach best meets user requirements?**
8. **What's the learning curve for each approach?**

### Nice-to-Know Questions

9. **Is there Ubuntu Touch roadmap for Bluetooth improvements?**
10. **Are there pending changes that might affect our approach?**
11. **What do other mobile Linux platforms use?**

## Success Criteria

### Minimum (Must Achieve)

- [ ] Understand why obexd unavailable/not working
- [ ] Identify at least one viable path forward
- [ ] Know why SharePlugin code doesn't execute
- [ ] Have clear recommendation for Sprint #005

### Target (Should Achieve)

- [ ] Document 2-3 alternative approaches with pros/cons
- [ ] Have working prototype demonstrating feasibility
- [ ] Understand ContentHub plugin architecture
- [ ] Know exact implementation steps for chosen approach

### Stretch (Nice to Achieve)

- [ ] Working minimal file transfer example
- [ ] Contact with Ubuntu Touch developers for guidance
- [ ] Contribution to Ubuntu Touch documentation
- [ ] Connection with other app developers

## Deliverables

### Documentation

1. **Research Reports** (per task)
   - `obex-investigation.md`
   - `contenthub-analysis.md`
   - `bluetooth-alternatives.md`
   - `working-examples.md`

2. **Decision Document**
   - `architectural-decision.md`
   - Chosen approach with rationale
   - Implementation roadmap
   - Risk assessment

3. **Sprint Summary**
   - `sprint-004-summary.md`
   - Findings recap
   - Lessons learned
   - Next steps

### Optional Deliverables

- Proof-of-concept code
- Test results with logs
- Screenshots/videos
- ADR (if architectural change significant)

## Timeline

**Total Estimated Time:** 20-28 hours

**Suggested Breakdown:**
- Week 1: Tasks 1 & 2 (OBEX + ContentHub investigation)
- Week 2: Tasks 3 & 4 (Alternatives + Testing)
- Week 3: Analysis, decision, documentation

**Flexible:** This is research - may finish early if findings are clear, or take longer if issues are complex.

## Resources Needed

### Access
- Ubuntu Touch device (Pixel 3a)
- Terminal/SSH access
- Root access (if available/needed)
- Development environment

### Documentation
- UBports developer docs
- Ubuntu Touch app examples
- Qt documentation
- BlueZ documentation
- OpenStore app list

### Tools
- adb/ssh for device access
- Log viewers
- D-Bus inspection tools
- Development tools for prototyping

### Community
- UBports forum access
- Telegram developer channels (if available)
- GitHub repositories of working apps

## Risk Assessment

### Research Risks

| Risk | Probability | Impact | Mitigation |
|------|------------|---------|------------|
| Cannot determine obexd viability | Low | High | Test multiple approaches in parallel |
| No working Bluetooth examples found | Medium | High | Research Qt alternatives thoroughly |
| ContentHub docs insufficient | Medium | Medium | Study working app source code |
| Device testing limited by time | Low | Medium | Prioritize critical tests first |
| Findings inconclusive | Low | High | Extend sprint if needed |

### Decision Risks

| Risk | Probability | Impact | Mitigation |
|------|------------|---------|------------|
| Choose approach that proves unfeasible | Medium | High | Prototype before committing |
| Platform limitations discovered late | Medium | High | Thorough research now prevents this |
| Architectural change too large | Low | High | Plan incremental migration |

## Out of Scope

**This sprint does NOT include:**
- ❌ Writing production code
- ❌ Fixing existing bugs
- ❌ Adding new features
- ❌ UI/UX improvements
- ❌ Performance optimization
- ❌ Translation updates

**Focus is 100% on research and understanding.**

## Dependencies

- Access to Ubuntu Touch device
- Ability to install/test apps
- Time to research and experiment
- UBports community resources

## Notes

### Why Research Sprint is Important

1. **Avoid Wasted Effort**
   - Sprint #003 implemented fixes that didn't work
   - Platform assumptions were wrong
   - Better to learn now than after more work

2. **Make Informed Decisions**
   - Multiple approaches possible
   - Each has trade-offs
   - Need data to choose wisely

3. **Prevent Future Surprises**
   - Understand platform limitations upfront
   - Identify risks before implementation
   - Plan around constraints

4. **Build Knowledge Base**
   - Document for future reference
   - Help other Ubuntu Touch developers
   - Contribute to community

### Success Definition

**This sprint succeeds if:**
- We understand WHY Sprint #003 approach failed
- We know HOW to implement file transfer that works
- We can confidently plan Sprint #005 implementation
- We avoid repeating similar mistakes

**This sprint fails if:**
- Still unclear why things don't work
- No viable path forward identified
- More questions than answers remain
- Same assumptions persist

## Next Steps After Sprint #004

### If Research Succeeds

**Sprint #005 Planning:**
1. Implement chosen approach
2. Create working prototype
3. Test on device
4. Iterate based on results
5. Target: Working file transfer

### If Research Reveals Blockers

**Options:**
1. Pivot to different approach (e.g., QtBluetooth-only)
2. Reduce scope (send-only, no receive)
3. Consider alternative platforms
4. Engage Ubuntu Touch developers for platform improvements

### If Multiple Approaches Viable

**Prototype Sprint:**
1. Implement multiple minimal prototypes
2. Compare on device
3. Choose best based on results
4. Proceed with winner

## Questions for Stakeholder

Before starting Sprint #004:

1. **Timeline:** Is research sprint acceptable, or pressure to deliver quickly?
2. **Scope:** Willing to reduce features if platform limited?
3. **Platform:** Committed to Ubuntu Touch, or open to alternatives?
4. **Help:** Should we seek Ubuntu Touch developer community assistance?

---

**Ready to begin research when you give the go-ahead!** 🔍
