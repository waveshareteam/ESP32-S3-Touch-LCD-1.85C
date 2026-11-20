# Firmware

[中文](README_ZH.md)

| File | Version / compatibility | Purpose | Flash offset |
| --- | --- | --- | --- |
| [ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20261022.bin](ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20261022.bin) | 2026-10-22; **V2 hardware only** | Factory test firmware; not compatible with V1 hardware | `0x0` |
| [ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20250227.bin](ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20250227.bin) | 2025-02-27; **V1 hardware only** | Factory test firmware | `0x0` |
| [dlna.bin](../Arduino/examples/DLNA/dlna.bin) | Legacy; hardware compatibility and version date not documented | Historical DLNA example image | `0x0` |

## Flash the V2 factory firmware

Target: ESP32-S3-Touch-LCD-1.85C **V2 only**, ESP32-S3, 16MB Flash. Do not flash this version onto V1 hardware.
The version date 2026-10-22 is supplied by the maintainer. The supplied test BIN is copied without changing its contents.

Erase Flash before flashing (this removes existing data). Replace `PORT` with the device serial port:

```sh
python -m esptool --chip esp32s3 --port PORT erase-flash
python -m esptool --chip esp32s3 --port PORT write-flash 0x0 ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20261022.bin
```

SHA-256: `441d34526d7f10798a3fa07e102628fde5ff8a636389d067dbdbd0dd440adeed`.

## Flash the V1 factory firmware

Use this image on ESP32-S3-Touch-LCD-1.85C V1 hardware only. The version date comes from the build date embedded in the image.

Replace PORT with the device serial port. Erasing flash clears existing data.

```sh
python -m esptool --chip esp32s3 --port PORT erase-flash
python -m esptool --chip esp32s3 --port PORT write-flash 0x0 ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20250227.bin
```

SHA-256: `13a8329e7479a5b6aa1eec4d6f0ea4b144e019a15f65eb164bbc8f1e948f5075`.

[Official hardware documentation](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C)
