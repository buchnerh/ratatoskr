# Sprint #008: SharePlugin improvement priorities
(pan, v1, 30/01/2026)

- [x] fix indentation issue in `ratatoskr/shareplugin/Main.qml` starting at line
  that contains:
  
  ```
  applicationName: "ratatoskr.philipa"
  ```

- [x] remove from `ratatoskr/shareplugin/Main.qml` those debugging messages 
  that do not bring real value

- [x] for the time being, remove completely the "file preview" feature to get
  rid of the black square in the top half of the screen (see 
  [screenshot](./screenshot20260129_135346499_scaled.jpg))

- [ ] the device list at present only contains MAC addresses -- it should also 
  show the device name
  
- [ ] if at all possible, the list should also allow to identify the devices
  that are already paired and/or connected over Bluetooth
  
