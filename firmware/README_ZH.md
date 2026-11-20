# 固件

[English](README.md)

| 文件 | 版本 / 适用范围 | 用途 | 烧录偏移 |
| --- | --- | --- | --- |
| [ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20261022.bin](ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20261022.bin) | 2026-10-22；**仅适用于 V2 硬件** | 出厂测试固件；不适用于 V1 硬件 | `0x0` |
| [ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20250227.bin](ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20250227.bin) | 2025-02-27；**仅适用于 V1 硬件** | 出厂测试固件 | `0x0` |
| [dlna.bin](../Arduino/examples/DLNA/dlna.bin) | 历史版本；硬件适用范围及版本日期未记录 | 历史 DLNA 示例镜像 | `0x0` |

## 烧录 V2 出厂固件

适用产品：ESP32-S3-Touch-LCD-1.85C **仅 V2 硬件**，ESP32-S3 芯片，16MB Flash。请勿将此版本烧录到 V1 硬件。
版本日期 2026-10-22 由维护者指定。提供的测试 BIN 原样复制，内容未修改。

烧录前整片擦除 Flash（会清除已有数据）。将 `PORT` 替换为设备串口：

```sh
python -m esptool --chip esp32s3 --port PORT erase-flash
python -m esptool --chip esp32s3 --port PORT write-flash 0x0 ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20261022.bin
```

SHA-256：`441d34526d7f10798a3fa07e102628fde5ff8a636389d067dbdbd0dd440adeed`。

## 烧录 V1 出厂固件

仅适用于 ESP32-S3-Touch-LCD-1.85C V1 硬件。版本日期取自镜像内的编译日期。

将 PORT 替换为设备串口。整片擦除会清除已有数据。

```sh
python -m esptool --chip esp32s3 --port PORT erase-flash
python -m esptool --chip esp32s3 --port PORT write-flash 0x0 ESP32-S3-Touch-LCD-1.85C_FactoryOnly_20250227.bin
```

SHA-256: `13a8329e7479a5b6aa1eec4d6f0ea4b144e019a15f65eb164bbc8f1e948f5075`.

[官方硬件资料](https://docs.waveshare.com/ESP32-S3-Touch-LCD-1.85C)
