# Sprint #007: 'ratatoskr' SharePlugin test report
(pan, v0, 23/01/2026)


Test performed using the following package: `ratatoskr.philipa_260122165030_arm64.click` 
(result of Sprint #007).

## Test steps and corresponding log entries

- installed latest package `ratatoskr.philipa_260122165030_arm64.click`

  Result:
  
  - installation successful
  - a new app called 'Bluetooth' appears in the app drawer
  - the new app 'Bluetooth' has no icon

- connect phone to laptop over BT through 'system settings' app:

  ```
  Jan 23 10:00:11 hatshepsut systemd[2136]: Started lomiri-app-launch--application-legacy--lomiri-system-settings--.service.
  ```

  Result: laptop connected, confirmed by status icon on both sides.

- open "File Manager" app:

  ```
  Jan 23 10:00:59 hatshepsut lomiri[3136]: [2026-01-23:10:00:59.921] qtmir.sessions: TaskController::onSessionStarting - sessionName=lomiri-filemanager-app
  ```

- select a random picture file below `Pictures/Others`

  ```
  Jan 23 10:01:09 hatshepsut aa-exec[4046]: qml: openFromDisk():/home/phablet/Pictures/Others/IMG_0446.jpg
  ```

- slide left, click on its "Share" icon 

  Result: the "Share to" selector appears with the following apps listed:
  
    - Messaging
    - Dekko 2
    - Bluetooth
    
- click on "Bluetooth":

  ```
  Jan 23 10:01:14 hatshepsut systemd[2136]: Started lomiri-app-launch--application-click--ratatoskr.philipa_shareplugin_260122165030--.service.
  ```

  Result:
  
    - a new app starts that takes the whole screen
    - the new app background is completely black with the "Bluetooth" word
      written in the middle of the screen
    - a rotating blue circle animation denotes waiting for something, but
      nothing more happens
    - the laptop doesn't seem to detect any BT transfer attempt
    
  Note:
  
    - the laptop's Bluetooth adaptor MAC address is `94:E6:F7:22:D6:56` (public)

- I eventually stopped the SharePlugin app manually:

  ```
  Jan 23 10:02:19 hatshepsut systemd[2136]: Stopping lomiri-app-launch--application-click--ratatoskr.philipa_shareplugin_260122165030--.service...
  ```

## Full 'journal' content for test duration

See: `docs/sprint-007/journal_cropped-20260123.txt`
