# 'ratatoskr' SharePlugin test report
(pan, v0, 21/01/2026)


Test performed using the following package: `ratatoskr.philipa_260121135941_arm64.click` 
(result of Sprint #005).

## Test steps and corresponding log entries

- connect phone to laptop over BT through 'system settings' app:

  ```
  Jan 21 16:40:21 hatshepsut lomiri[4657]: [2026-01-21:16:40:21.610] qtmir.sessions: TaskController::onSessionStarting - sessionName=lomiri-system-settings
  ```

  Result: laptop connected, confirmed by status icon on both sides.

- verify that OBEX daemon is still running through 'shell' app:

  ```
  Jan 21 16:40:53 hatshepsut lomiri[4657]: [2026-01-21:16:40:53.664] qtmir.sessions: TaskController::onSessionStarting - sessionName=lomiri-terminal-app
  ```

  Result: `obexd` still running.
  
- open "Contacts" app:

  ```
  Jan 21 16:41:47 hatshepsut systemd[2232]: Started lomiri-app-launch--application-legacy--address-book-app--.service.
  ```

- open a random contact in the list
- click on its "Share" icon 

  Result: the "Share to" selector appears with the following apps listed:
  
    - Messaging
    - File Manager
    - Dekko 2
    - ratatoskr.philipa_shareplu
    
- click on "ratatoskr.philipa_shareplu":

  ```
  Jan 21 16:42:21 hatshepsut com.lomiri.content.dbus.Service[3178]: QMap<QString, QString> {anonymous}::info_for_app_id(QString) Failed to create Application for "ratatoskr.philipa_shareplugin_260121135941"
  ```

  Result:
  
    - a pop-up appears for a fraction of a second (too fast to read what it 
      says)
    - the phone restarts (soft restart)
    - the laptop doesn't seem to detect any BT transfer attempt
    
## Full 'journal' content for test duration

See: `docs/sprint-006/journal_cropped-20260121.txt`
