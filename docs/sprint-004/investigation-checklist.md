# Sprint #004 Investigation Checklist

**Purpose:** Track research progress and ensure all questions answered

## OBEX Investigation Checklist

### Package and Installation
- [ ] Run `dpkg -l | grep -i obex`
- [ ] Run `apt search obex`
- [ ] Document installed packages
- [ ] Check package versions
- [ ] Note any dependencies

### Service Status
- [ ] Check `systemctl list-units | grep obex`
- [ ] Try `systemctl status obexd`
- [ ] Check D-Bus services: `dbus-send --system --print-reply --dest=org.freedesktop.DBus /org/freedesktop/DBus org.freedesktop.DBus.ListNames | grep obex`
- [ ] Document why service not running

### Configuration Files
- [ ] Find obex-related files: `find /etc /usr -name "*obex*" 2>/dev/null`
- [ ] Check D-Bus service files: `cat /usr/share/dbus-1/services/*obex*`
- [ ] Check systemd units: `cat /lib/systemd/system/*obex*`
- [ ] Document configuration structure

### Manual Start Attempts
- [ ] Locate obexd binary: `which obexd`
- [ ] Check help: `/usr/libexec/bluetooth/obexd --help`
- [ ] Try manual start: `/usr/libexec/bluetooth/obexd --systemd --debug`
- [ ] Document any errors
- [ ] Check AppArmor restrictions

### Conclusion
- [ ] Write findings in `obex-investigation.md`
- [ ] Answer: Can we use obexd? Yes/No
- [ ] If Yes: Document how to enable
- [ ] If No: Document why not

---

## ContentHub Investigation Checklist

### Our Installation
- [ ] List installed files: `ls -la /opt/click.ubuntu.com/ratatoskr.philipa/current/`
- [ ] Check manifest: `cat /opt/click.ubuntu.com/ratatoskr.philipa/current/manifest.json`
- [ ] Check desktop files: `ls -la /home/phablet/.local/share/applications/ratatoskr*`
- [ ] Read SharePlugin desktop: `cat /home/phablet/.local/share/applications/ratatoskr.philipa_shareplugin*.desktop`
- [ ] Verify binary exists and is executable

### Binary Testing
- [ ] Run SharePlugin directly: `/opt/click.ubuntu.com/ratatoskr.philipa/current/shareplugin`
- [ ] Does it launch?
- [ ] Does UI appear?
- [ ] Any errors in terminal?
- [ ] Check logs: `journalctl -f | grep ratatoskr`

### Working Examples Study
- [ ] Identify 3+ apps with ContentHub integration
- [ ] For each app, examine:
  - [ ] `ls -la /opt/click.ubuntu.com/[app]/current/`
  - [ ] `cat manifest.json`
  - [ ] `cat *.desktop`
  - [ ] `file [binary]` - Is it executable?
- [ ] Document patterns found
- [ ] Identify differences from our approach

### ContentHub Documentation
- [ ] Read UBports ContentHub docs
- [ ] Search Ubuntu Touch forums for ContentHub discussions
- [ ] Look for ContentHub examples in GitHub
- [ ] Note any version differences (our focal vs older)

### Runtime Monitoring
- [ ] Monitor ContentHub during invocation: `journalctl -f | grep -i content`
- [ ] Monitor D-Bus: `dbus-monitor --session | grep -i content`
- [ ] Capture full invocation sequence
- [ ] Document what actually happens

### Conclusion
- [ ] Write findings in `contenthub-analysis.md`
- [ ] Answer: Why doesn't our code run?
- [ ] Identify correct implementation pattern
- [ ] Document changes needed

---

## Bluetooth Alternatives Checklist

### Qt Bluetooth Research
- [ ] Review QBluetoothServer documentation
- [ ] Review QBluetoothTransferManager documentation
- [ ] Check what protocols Qt supports (OBEX, RFCOMM, L2CAP)
- [ ] Note any Ubuntu Touch specific limitations
- [ ] Identify examples in Qt docs

### OpenStore App Analysis
- [ ] Search OpenStore for "bluetooth"
- [ ] Search OpenStore for "file transfer"
- [ ] Identify apps that do Bluetooth file transfer
- [ ] For each app found:
  - [ ] Install and test
  - [ ] Document how it works
  - [ ] Check if source available
  - [ ] Identify approach used

### Community Research
- [ ] Search UBports forum: "bluetooth file transfer"
- [ ] Search UBports forum: "obex"
- [ ] Look for GitHub repos of Bluetooth apps
- [ ] Check Telegram channels (if available)
- [ ] Ask questions if needed

### Protocol Alternatives
- [ ] Can we use RFCOMM instead of OBEX?
- [ ] What about L2CAP?
- [ ] WebBluetooth feasibility?
- [ ] Trade-offs of each approach

### Conclusion
- [ ] Write findings in `bluetooth-alternatives.md`
- [ ] Create comparison matrix
- [ ] List pros/cons of each approach
- [ ] Recommend 1-2 approaches to prototype

---

## Device Testing Checklist

### OBEX Service Tests
- [ ] If obexd can be enabled, document steps
- [ ] Test file receiving with obexd running
- [ ] Capture detailed logs
- [ ] Document success/failure
- [ ] Note any AppArmor issues

### SharePlugin Binary Tests
- [ ] Modify SharePlugin with obvious change (e.g., window title)
- [ ] Rebuild and reinstall
- [ ] Invoke from ContentHub
- [ ] Does change appear?
- [ ] Document what happens

### QtBluetooth Prototype
- [ ] Create minimal test app
- [ ] Implement basic Bluetooth listening
- [ ] Test on device
- [ ] Can it receive connections?
- [ ] Document capabilities and limitations

### ContentHub Minimal Test
- [ ] Create simplest possible ContentHub plugin
- [ ] Just show "Hello World" window
- [ ] Test if ContentHub invokes it
- [ ] Gradually add complexity
- [ ] Identify what works and what breaks

### Conclusion
- [ ] Document all test results
- [ ] Capture screenshots/photos
- [ ] Save all logs
- [ ] Validate or invalidate theories

---

## Critical Questions Status

### Must Answer
- [ ] **Q1:** Is obexd available on Ubuntu Touch?
  - Answer: _______________
- [ ] **Q2:** Can we start obexd service from app?
  - Answer: _______________
- [ ] **Q3:** Why doesn't ContentHub execute our binary?
  - Answer: _______________
- [ ] **Q4:** What do working Bluetooth apps use?
  - Answer: _______________

### Should Answer
- [ ] **Q5:** Can QtBluetooth replace obexd for our use case?
  - Answer: _______________
- [ ] **Q6:** What are limitations of each approach?
  - Answer: _______________
- [ ] **Q7:** Which approach best meets user requirements?
  - Answer: _______________
- [ ] **Q8:** What's the learning curve for each approach?
  - Answer: _______________

---

## Final Deliverables Checklist

### Required Documents
- [ ] `obex-investigation.md` - Complete with evidence
- [ ] `contenthub-analysis.md` - Complete with examples
- [ ] `bluetooth-alternatives.md` - Complete with matrix
- [ ] `working-examples.md` - Document studied apps
- [ ] `architectural-decision.md` - Clear recommendation
- [ ] `sprint-004-summary.md` - Overall findings

### Supporting Materials
- [ ] Test logs collected
- [ ] Screenshots captured
- [ ] Code snippets documented
- [ ] Commands and results recorded

### Decision Made
- [ ] Chosen approach clearly identified
- [ ] Rationale documented
- [ ] Trade-offs understood
- [ ] Implementation plan outlined
- [ ] Ready to plan Sprint #005

---

## Progress Tracking

**Started:** ___________  
**Expected Completion:** ___________  
**Actual Completion:** ___________

**Status:**
- [ ] Week 1: OBEX + ContentHub (Tasks 1-2)
- [ ] Week 2: Alternatives + Testing (Tasks 3-4)
- [ ] Week 3: Analysis + Decision + Docs

**Blockers:** _______________________________________________

**Notes:** __________________________________________________

---

**Use this checklist to ensure thorough investigation!** ✓
