# Sprint #004: OBEX Investigation Result
(pan, v2, 15/01/2026)

## Package and Installation

- OBEX is installed, the following packages are present:

  | package      | version |
  | ------------ | ------- |
  | bluez-obexd  | 5.64    |
  | libopenobex2 | 1.7.2   |
  
- the `bluez-obexd` package brings the following files:

  ```
  /.
  /usr
  /usr/lib
  /usr/lib/bluetooth
  /usr/lib/bluetooth/obexd
  /usr/lib/systemd
  /usr/lib/systemd/user
  /usr/lib/systemd/user/obex.service
  /usr/share
  /usr/share/dbus-1
  /usr/share/dbus-1/services
  /usr/share/dbus-1/services/org.bluez.obex.service
  /usr/share/doc
  /usr/share/doc/bluez-obexd
  /usr/share/doc/bluez-obexd/NEWS.Debian.gz
  /usr/share/doc/bluez-obexd/changelog.Debian.gz
  /usr/share/doc/bluez-obexd/copyright
  /usr/lib/systemd/user/dbus-org.bluez.obex.service
  ```

- the `libopenobex2` package brings the following files:

  ```
  /.
  /lib
  /lib/udev
  /lib/udev/rules.d
  /lib/udev/rules.d/60-openobex.rules
  /usr
  /usr/lib
  /usr/lib/aarch64-linux-gnu
  /usr/lib/aarch64-linux-gnu/libopenobex.so.1.7.2
  /usr/sbin
  /usr/sbin/obex-check-device
  /usr/share
  /usr/share/doc
  /usr/share/doc/libopenobex2
  /usr/share/doc/libopenobex2/changelog.Debian.gz
  /usr/share/doc/libopenobex2/copyright
  /usr/lib/aarch64-linux-gnu/libopenobex.so.2
  ```

## Service Status

- `obexd` doesn't start automatically when the phone boots
- unit file `dbus-org.bluez.obex.service` is loaded but disabled
  - output from `systemctl status`:

    ```
    ● obex.service - Bluetooth OBEX service
         Loaded: loaded (/usr/lib/systemd/user/obex.service; disabled; vendor preset: enabled)
        Drop-In: /usr/lib/systemd/user/obex.service.d
                 └─ubuntu-touch-session.conf
         Active: inactive (dead)
    ```

## Configuration Files

- the OBEX daemon doesn't use configuration files as such
- the `systemd` unit file contains the following parameters:
 
  ```
  [Unit]
  Description=Bluetooth OBEX service

  [Service]
  Type=dbus
  BusName=org.bluez.obex
  ExecStart=/usr/lib/bluetooth/obexd

  [Install]
  Alias=dbus-org.bluez.obex.service
  ```

- the `systemd` unit file uses its own configuration file:
  - located below `/usr/lib/systemd/user/obex.service.d`
  - called `ubuntu-touch-session.conf`
  - provided by package `ubuntu-touch-session`
  - content:

    ```
    [Service]
    ExecStart=
    ExecStart=/usr/lib/bluetooth/obexd -P ftp,irmc,mas,pcsuite -r $HOME
    ```

## Manual Start Attempts

- OBEX daemon can be started with the following command: `systemctl --user start obex.service`
- once started, `obexd` keeps running
- it uses the command-line parameters specified by the 'systemd' unit configuration file: `-P ftp,irmc,mas,pcsuite -r /home/phablet`

## D-Bus investigation

- output of `busctl list | grep bluez`:

  ```
  org.bluez                                   2170 bluetoothd      root             :1.40         bluetooth.service               -       -
  ```

- output of `busctl list | grep obex` (while `obexd` is running):

  ```
  :1.129                                      5035 obexd           phablet          :1.129        user@32011.service              -       -
  ```


## New test of latest `ratatoskr` build with `obexd` running

- re-paired phone with laptop
- both devices show as connected
- manually started `obexd` with `systemctl --user start obex.service`
- tried to share `.jpg` file over BT from phone to PC, selected 'ratatoskr' as 
  plugin => phone restarted as before
- `obexd` was still running after restart, BT icon still indicated connection
- start main `ratatoskr` app
- share `.txt` file from PC to phone
- transfer looks OK from PC side (better than before!)
- nothing appears in "reception area" of `ratatoskr` app
- `obexd` log shows the following:

  ```
  [03/01/2026 12:15] OBEX daemon 5.64
  [03/01/2026 12:15] Excluding pcsuite
  [03/01/2026 12:15] Excluding ftp
  [03/01/2026 12:15] Excluding irmc
  [03/01/2026 12:15] Excluding mas
  [03/01/2026 12:23] CONNECT(0x0), <unknown>(0xff)
  [03/01/2026 12:23] CONNECT(0x0), <unknown>(0x0)
  [03/01/2026 12:23] PUT(0x2), <unknown>(0xff)
  [03/01/2026 12:23] PUT(0x2), Forbidden(0x43)
  [03/01/2026 12:23] DISCONNECT(0x1), <unknown>(0xff)
  [03/01/2026 12:23] DISCONNECT(0x1), Success(0x20)
  [03/01/2026 12:23] disconnected: Transport got disconnected
  [03/01/2026 12:25] CONNECT(0x0), <unknown>(0xff)
  [03/01/2026 12:25] CONNECT(0x0), <unknown>(0x0)
  [03/01/2026 12:25] PUT(0x2), <unknown>(0xff)
  [03/01/2026 12:25] PUT(0x2), Forbidden(0x43)
  [03/01/2026 12:25] DISCONNECT(0x1), <unknown>(0xff)
  [03/01/2026 12:25] DISCONNECT(0x1), Success(0x20)
  [03/01/2026 12:25] disconnected: Transport got disconnected
  [03/01/2026 12:32] Terminating
  ```

- the following also appears in 'journal' shortly after each phone startup
  (even when no tests are played and `obexd` is not started):
  
  ```
  Jan 09 08:20:50 hatshepsut lomiri[3144]: [2026-01-09:08:20:50.370] lomiri-app-launch threw an exception getting app info for appId: "ratatoskr.philipa_shareplugin" : Application is not shown in 'Lomiri'
  Jan 09 08:20:50 hatshepsut lomiri[3144]: [2026-01-09:08:20:50.370] Failed to get app info for app "ratatoskr.philipa_shareplugin"
  [...]
  Jan 09 08:20:54 hatshepsut com.lomiri.content.dbus.Service[3204]: QMap<QString, QString> {anonymous}::info_for_app_id(QString) Failed to create Application for "ratatoskr.philipa_shareplugin_251225154300"
  Jan 09 08:20:54 hatshepsut com.lomiri.content.dbus.Service[3204]: QMap<QString, QString> {anonymous}::info_for_app_id(QString) Failed to create Application for "ratatoskr.philipa_shareplugin_251225154300"
  ```

## Second test of latest `ratatoskr` build after enabling `obex.service` unit

- enabled `obex.service` unit with `systemctl --user enable obex.service`
- started `obexd` with `systemctl --user start obex.service`
- checked that service was enabled with `systemctl --user status obex.service`
- performed full power-cycle on phone
- after reboot, `obexd` was NOT running (reason unknown)
- verified that `obex.service` unit was still enabled: it was
- manually started `obexd` with `systemctl --user start obex.service`
- re-did BT transfer test from laptop to phone
- same behaviour as above (OK on laptop side, no reaction on phone)
- same messages as above in the application logs (obex, ratatoskr)
- here below all relevant entries from 'journal':

  ```
  Jan 15 10:58:08 hatshepsut lomiri[3103]: [2026-01-15:10:58:08.511] lomiri-app-launch threw an exception getting app info for appId: "ratatoskr.philipa_shareplugin" : Application is not shown in 'Lomiri'
  Jan 15 10:58:08 hatshepsut lomiri[3103]: [2026-01-15:10:58:08.511] Failed to get app info for app "ratatoskr.philipa_shareplugin"

  Jan 15 10:58:12 hatshepsut com.lomiri.content.dbus.Service[3167]: QMap<QString, QString> {anonymous}::info_for_app_id(QString) Failed to create Application for "ratatoskr.philipa_shareplugin_251225154300"
  Jan 15 10:58:13 hatshepsut com.lomiri.content.dbus.Service[3167]: QMap<QString, QString> {anonymous}::info_for_app_id(QString) Failed to create Application for "ratatoskr.philipa_shareplugin_251225154300"
  Jan 15 10:58:13 hatshepsut com.lomiri.content.dbus.Service[3167]: QMap<QString, QString> {anonymous}::info_for_app_id(QString) Failed to create Application for "ratatoskr.philipa_shareplugin_251225154300"
  Jan 15 10:58:13 hatshepsut com.lomiri.content.dbus.Service[3167]: QMap<QString, QString> {anonymous}::info_for_app_id(QString) Failed to create Application for "ratatoskr.philipa_shareplugin_251225154300"
  Jan 15 10:58:13 hatshepsut com.lomiri.content.dbus.Service[3167]: QMap<QString, QString> {anonymous}::info_for_app_id(QString) Failed to create Application for "ratatoskr.philipa_shareplugin_251225154300"
  Jan 15 10:58:13 hatshepsut com.lomiri.content.dbus.Service[3167]: QMap<QString, QString> {anonymous}::info_for_app_id(QString) Failed to create Application for "ratatoskr.philipa_shareplugin_251225154300"

  Jan 15 11:05:00 hatshepsut systemd[2193]: Started lomiri-app-launch--application-click--ratatoskr.philipa_ratatoskr_251225154300--.service.
  Jan 15 11:05:00 hatshepsut lomiri-system-settings[3579]: QObject::startTimer: Timers cannot be started from another thread
  Jan 15 11:05:00 hatshepsut lomiri[3103]: [2026-01-15:11:05:00.456] qtmir.mir: SessionAuthorizer::connection_is_allowed - this= qtmir::SessionAuthorizer(0x7cec1a7ab0) pid= 3814
  Jan 15 11:05:00 hatshepsut lomiri[3103]: [2026-01-15:11:05:00.460] qtmir.mir: SessionAuthorizer::configure_input_is_allowed - this= qtmir::SessionAuthorizer(0x7cec1a7ab0) pid= 3814
  Jan 15 11:05:00 hatshepsut lomiri[3103]: [2026-01-15:11:05:00.460] qtmir.mir: SessionAuthorizer::set_base_input_configuration_is_allowed - this= qtmir::SessionAuthorizer(0x7cec1a7ab0) pid= 3814
  Jan 15 11:05:00 hatshepsut lomiri[3103]: [2026-01-15:11:05:00.460] qtmir.mir: SessionAuthorizer::configure_display_is_allowed - this= qtmir::SessionAuthorizer(0x7cec1a7ab0) pid= 3814
  Jan 15 11:05:00 hatshepsut lomiri[3103]: [2026-01-15:11:05:00.460] qtmir.mir: SessionAuthorizer::set_base_display_configuration_is_allowed - this= qtmir::SessionAuthorizer(0x7cec1a7ab0) pid= 3814
  Jan 15 11:05:00 hatshepsut lomiri[3103]: [2026-01-15:11:05:00.460] qtmir.mir: SessionAuthorizer::screencast_is_allowed - this= qtmir::SessionAuthorizer(0x7cec1a7ab0) pid= 3814
  Jan 15 11:05:00 hatshepsut lomiri[3103]: [2026-01-15:11:05:00.460] qtmir.mir: SessionAuthorizer::prompt_session_is_allowed - this= qtmir::SessionAuthorizer(0x7cec1a7ab0) pid= 3814
  Jan 15 11:05:00 hatshepsut lomiri[3103]: [2026-01-15:11:05:00.465] qtmir.sessions: TaskController::onSessionStarting - sessionName=ratatoskr
  Jan 15 11:05:00 hatshepsut aa-exec[3814]: Creating a QMirClientScreen now
  Jan 15 11:05:00 hatshepsut aa-exec[3814]: reply QDBusMessage(type=MethodReturn, service=":1.28", signature="", contents=([]) )
  Jan 15 11:05:00 hatshepsut aa-exec[3814]: creating agent on dbus
  Jan 15 11:05:00 hatshepsut aa-exec[3814]: registering agent
  Jan 15 11:05:00 hatshepsut aa-exec[3814]: registering agent on obexd-server
  Jan 15 11:05:00 hatshepsut aa-exec[3814]: Error registering agent for the default adapter: QDBusError("org.freedesktop.DBus.Error.ServiceUnknown", "The name org.bluez.obex was not provided by any .service files")
  Jan 15 11:05:00 hatshepsut aa-exec[3814]: have entries: (".", "..", "HubIncoming") "/home/phablet/.cache/ratatoskr.philipa"
  Jan 15 11:05:01 hatshepsut audit[3814]: AVC apparmor="DENIED" operation="open" profile="ratatoskr.philipa_ratatoskr_251225154300" name="/etc/ubuntu-touch-session.d/android.conf" pid=3814 comm=52504320546872656164 requested_mask="r" denied_mask="r" fsuid=32011 ouid=0
  Jan 15 11:05:01 hatshepsut audit[3814]: AVC apparmor="DENIED" operation="open" profile="ratatoskr.philipa_ratatoskr_251225154300" name="/proc/3814/loginuid" pid=3814 comm="ratatoskr" requested_mask="r" denied_mask="r" fsuid=32011 ouid=32011
  Jan 15 11:05:01 hatshepsut audit[3814]: AVC apparmor="DENIED" operation="open" profile="ratatoskr.philipa_ratatoskr_251225154300" name="/home/phablet/.cache/qtshadercache-arm64-little_endian-lp64/7d5f9fc417d36376dbfbebf864483aca3a25ac1a" pid=3814 comm="QSGRenderThread" requested_mask="r" denied_mask="r" fsuid=32011 ouid=32011
  Jan 15 11:05:01 hatshepsut audit[3814]: AVC apparmor="DENIED" operation="open" profile="ratatoskr.philipa_ratatoskr_251225154300" name="/home/phablet/.cache/qtshadercache-arm64-little_endian-lp64/#2361655" pid=3814 comm="QSGRenderThread" requested_mask="wr" denied_mask="wr" fsuid=32011 ouid=32011

  Jan 15 11:06:15 hatshepsut lomiri[3103]: [2026-01-15:11:06:15.018] qtmir.sessions: TaskController::onSessionStopping - sessionName=ratatoskr
  Jan 15 11:06:15 hatshepsut lomiri-system-settings[3579]: Failed to retrieve properties for device "/org/bluez/hci0/dev_4F_D4_71_8A_B5_73"
  Jan 15 11:06:15 hatshepsut lomiri-system-settings[3579]: Failed to retrieve properties for device "/org/bluez/hci0/dev_69_74_6C_6F_90_ED"
  Jan 15 11:06:15 hatshepsut systemd[2193]: lomiri-app-launch--application-click--ratatoskr.philipa_ratatoskr_251225154300--.service: Succeeded.
  Jan 15 11:06:15 hatshepsut lomiri[3103]: [2026-01-15:11:06:15.054] ApplicationManager::onProcessStopped reports stop of appId= "ratatoskr.philipa_ratatoskr" which AppMan is not managing, ignoring the event
  Jan 15 11:06:23 hatshepsut lomiri[3103]: [2026-01-15:11:06:23.570] Input device removed: "kermit (AVRCP)" "/dev/input/input8" QFlags<QInputDevice::InputType>(Button)

  Jan 15 11:05:48 hatshepsut bluetoothd[2030]: src/profile.c:ext_io_disconnected() Unable to get io data for Object Push: getpeername: Transport endpoint is not connected (107)
  ```

## Conclusions and hypotheses

- when `obexd` is started manually, it seems to detect and handle the transfer
  initiated by the PC, but it may lack the ability to hand it over (through 
  D-Bus) to the `ratatoskr` app
  
- by comparing the `busctl list` output between the phone and my laptop, I 
  have the feeling that only processes started by 'root' will register a name
  for their service (e.g. `org.bluez`) whereas user processes are limited to 
  numerical addresses (e.g. `:1.129`), which may change between sessions. 
  
  Maybe there is way to query D-Bus for the numerical address used by `obexd`
  which could be added before calling `m_manager.call()` in `obexd.cpp`?
  
- the error message returned by `m_manager.call()`:

  ```
  QDBusError("org.freedesktop.DBus.Error.ServiceUnknown", "The name org.bluez.obex was not provided by any .service files")
  ```

  clearly refers to a name ("org.bluez.obex") expected to be found in the unit's
  `.service` file. That string is indeed present in `/usr/lib/systemd/user/obex.service`
  but defined as a `BusName` (as expected when `Type=dbus`), not a `Name`.

- the 'journal' entries quoted in the first test report above may indicate that 
  the `ratatoskr` share plugin should register with Lomiri somehow
  
