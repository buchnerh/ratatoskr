# Sprint #002: test result
(pan, 21/12/2025)

## Compilation / auto-review on host arch.

- Build successful
- Auto-review produces the following errors (but they were expected):

  ```
   - security:policy_groups_safe:ratatoskr:bluetooth
	  (NEEDS REVIEW) reserved policy group 'bluetooth': vetted applications only
   - security:policy_groups_safe:shareplugin:bluetooth
	  (NEEDS REVIEW) reserved policy group 'bluetooth': vetted applications only
  ```
  
## Start application inside emulator

- MainWindow displays as expected: [screenshot](./MainWindow_EMU-002.jpg)
- NearbyDevices screen displays when the top-right icon is clicked: [screenshot](./NearbyDevice_EMU-002.jpg)

## Compilation / auto-review for target arch. (`arm64`)

- Build successful
- Generated installation package `./ratatoskr/build/aarch64-linux-gnu/app/ratatoskr.philipa_251220171217_arm64.click`
- Auto-review produces the same errors as above (just as expected)

## Installation on target device

- Using the standard approach (`clickable install`) failed as the phone was
  not detected (`clickable devices` returns "No attached devices"), although
  `adb devices` successfully detects it.
- Installation successfully performed using an alternative approach:
  - copied `arm64` click file to phone over USB
  - tapped click file on phone
  - selected "install with other application"
  - selected "OpenStore"
  - accepted disclaimer about risks
  - package installation proceeded and was reported successful
  - 'Ratatoskr' icon showed up on phone's desktop
- Note: during installation, no prompt was offered to confirm special rights
  for application (e.g. access to bluetooth)

## Start application on phone

- MainWindow displays as expected, same layout as inside emulator
- NearbyDevices screen displays when the top-right icon is clicked
- NearbyDevices screen lists known paired devices (the Skoda car kit and
  the Sena headset of my motorbike helmet) [screenshot](./screenshot20251221_182809217.jpg)

## Try using the SharePlugin to push Contact card to phone

- In Contact app, open a random contact
- Tap "share" icon in top-right corner
- 'ratatoskr.philipa_shareplu' appears in the list of available apps as expected
- 'ratatoskr.philipa_shareplu' has a blank icon, although it was expected to 
  the one defined for the main app [screenshot](./screenshot20251221_183833900.jpg)
- tapping on blank 'ratatoskr' icon causes phone to reboot instantly
- [log file](./lomiri-app-launch--application-click--ratatoskr.philipa_ratatoskr_251220171217--.service)
  indicates that access to Bluetooth stack has been denied by AppArmor
- Looking at 'Ratatoskr' app permissions in "UT Tweak Tool" indeed doesn't list
  Bluetooth

