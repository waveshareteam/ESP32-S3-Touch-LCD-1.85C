# Firmware

[中文](README_ZH.md)

| File | Version / compatibility | Purpose | Flash offset |
| --- | --- | --- | --- |
| [ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20261022.bin](ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20261022.bin) | 2026-10-22; **V2 hardware only** | Factory test firmware; not compatible with V1 hardware | `0x0` |
| [dlna.bin](../Arduino/examples/DLNA/dlna.bin) | Legacy; hardware compatibility and version date not documented | Historical DLNA example image | `0x0` |

## Flash the latest factory firmware

Target: ESP32-S3-Touch-LCD-1.85C **V2 only**, ESP32-S3, 16MB Flash. Do not flash this version onto V1 hardware.
The version date 2026-10-22 is supplied by the maintainer. The supplied test BIN is copied without changing its contents.

Erase Flash before flashing (this removes existing data). Replace `PORT` with the device serial port:

```sh
python -m esptool --chip esp32s3 --port PORT erase-flash
python -m esptool --chip esp32s3 --port PORT write-flash 0x0 ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20261022.bin
```

SHA-256: `441d34526d7f10798a3fa07e102628fde5ff8a636389d067dbdbd0dd440adeed`.

[Official hardware documentation](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C)
