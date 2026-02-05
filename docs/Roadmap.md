# Roadmap for `ratatoskr` app
(pan, v2, 30/01/2026)


## Generalities

- [x] Contact Michael Zanetti to determine appropriate license
- [ ] Update ADR-005 with final display issue resolution (`Window` object)

## Enhancements

### Main app

- [ ] Add confirmation pop-up before deleting received file from ItemList
- [ ] Add confirmation pop-up before accepting incoming file over BT
- [ ] Get rid of 'unconfined' template in AA profile (may require platform 
  changes)
- [ ] Code cleanup: remove objects that are not needed (BluetoothManager?,
  DeviceDiscovery?, OBEXTransferService?)

### SharePlugin

- [x] Rename "Bluetooth" desktop icon to "BT Plugin"
- [ ] Find way of hiding plugin icon on desktop -- may require to redesign
  plugin as Headless ContentHub peer (see trace from Sprint #006)
- [ ] Get rid of 'unconfined' template in AA profile (may require platform 
  changes)
- [ ] if at all possible, the list should also allow to identify the devices
  that are already paired and/or connected over Bluetooth

## Bugs

### Main app

### SharePlugin

- [x] Plugin crashes and causes phone to restart
- [ ] Plugin has no icon -- should use `ratatoskr/shareplugin/icon.svg` (BT 
  logo)
- [x] the device list at present only contains MAC addresses -- it should also 
  show the device name
