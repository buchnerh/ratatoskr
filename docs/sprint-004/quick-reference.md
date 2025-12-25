# Sprint #004 - Quick Reference

**Type:** Research Sprint  
**Duration:** 2-3 weeks  
**Target Version:** 0.0.3 (unchanged)  
**Focus:** Understanding, not implementation

## What Is This Sprint?

A **research-only sprint** to investigate why Sprint #003 fixes didn't work and determine the correct implementation approach for Ubuntu Touch.

## The Problems We're Investigating

### Problem 1: OBEX Daemon Missing
```
Error: "The name org.bluez.obex was not provided by any .service files"
```
**Question:** Why? How to fix?

### Problem 2: SharePlugin Code Not Running
- Our fixes don't apply
- Same errors persist
- Icon fix didn't work

**Question:** Why? What's the right approach?

## Four Research Tasks

| # | Task | Focus | Time |
|---|------|-------|------|
| 1 | OBEX Investigation | Is it available? Can we use it? | 4-6h |
| 2 | ContentHub Analysis | How do plugins work? | 6-8h |
| 3 | Alternatives Research | What else can we use? | 4-6h |
| 4 | Device Testing | Validate findings | 6-8h |

**Total:** 20-28 hours

## Key Questions to Answer

### Must Answer
- [ ] Is obexd available on Ubuntu Touch?
- [ ] Can we start obexd from our app?
- [ ] Why doesn't ContentHub run our binary?
- [ ] What do working Bluetooth apps use?

### Should Answer
- [ ] Can QtBluetooth replace obexd?
- [ ] What are the limitations?
- [ ] Which approach best fits our needs?

## Investigation Commands

### On Device

```bash
# Check obexd
dpkg -l | grep -i obex
systemctl list-units | grep obex

# Check ContentHub registration
ls -la /home/phablet/.local/share/applications/ratatoskr*
cat /home/phablet/.local/share/applications/ratatoskr.philipa_shareplugin*.desktop

# Test binary directly
/opt/click.ubuntu.com/ratatoskr.philipa/current/shareplugin

# Monitor ContentHub
journalctl -f | grep -i content
```

### Research Activities

```bash
# Find Bluetooth apps
open-store search "bluetooth"

# Study working apps
ls -la /opt/click.ubuntu.com/[app-name]/current/
cat manifest.json

# Check D-Bus services
dbus-send --system --print-reply \
  --dest=org.freedesktop.DBus \
  /org/freedesktop/DBus \
  org.freedesktop.DBus.ListNames | grep -i blue
```

## Success Criteria

✅ **Sprint Succeeds If:**
- Understand why Sprint #003 failed
- Know how to implement working solution
- Can confidently plan Sprint #005
- Have clear architectural decision

❌ **Sprint Fails If:**
- Still unclear why things don't work
- No viable approach identified
- Same assumptions remain

## Deliverables

### Required Documents
1. `obex-investigation.md` - OBEX findings
2. `contenthub-analysis.md` - ContentHub architecture
3. `bluetooth-alternatives.md` - Alternative approaches
4. `architectural-decision.md` - Chosen path forward
5. `sprint-004-summary.md` - Overall findings

### Optional
- Proof-of-concept code
- Test logs and screenshots
- Working examples

## Timeline

**Week 1:** OBEX + ContentHub investigation  
**Week 2:** Alternatives + Device testing  
**Week 3:** Analysis + Decision + Documentation

**Flexible:** Research takes as long as needed

## What We're NOT Doing

❌ Writing production code  
❌ Fixing bugs  
❌ Adding features  
❌ UI improvements  
❌ Optimization

✅ Only: Research, learning, understanding, deciding

## After This Sprint

### If Successful
→ Sprint #005: Implement chosen approach

### If Blockers Found
→ Pivot strategy or reduce scope

### If Multiple Options
→ Prototype sprint to compare

## Key Insights So Far

1. **Ubuntu Touch ≠ Desktop Linux**
   - Different service availability
   - Different restrictions
   - Need platform-specific approach

2. **ContentHub Integration Complex**
   - Plugin architecture not well understood
   - Working examples needed
   - May need different approach

3. **OBEX May Not Be Viable**
   - Service not available
   - QtBluetooth might be alternative
   - Need to validate both approaches

## Resources

- UBports developer docs
- OpenStore app examples
- Qt Bluetooth documentation
- Device for testing

## Decision Framework

For each approach, evaluate:
- ✅ Feasibility (can we do it?)
- ✅ Complexity (how hard?)
- ✅ Limitations (what can't it do?)
- ✅ User impact (meets requirements?)
- ✅ Maintenance (long-term viable?)

## Questions Before Starting

1. Is research sprint timeline acceptable?
2. Open to reducing scope if platform limited?
3. Should we engage Ubuntu Touch community?
4. Commitment to Ubuntu Touch vs explore alternatives?

---

**Remember:** Research sprint success = gaining knowledge, not writing code.
