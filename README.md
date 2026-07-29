# ATBM6441 SDIO WiFi driver

This repository holds the kernel driver source for the ATBM6441 combo
SDIO WiFi and MCU chip.

## Scope

This tree holds only the files a build actually needs:

- `hal_apollo/` — the driver source. It builds the kernel module.
- `os/linux/` — one header. The driver includes it.
- `include/` — kernel compatibility headers. The build includes them.
- `Makefile`, `Makefile.build`, `Makefile.build.kernel` — the build
  files the kernel's external-module convention needs.

This tree does not include the vendor SDK's own build tools, prebuilt
binaries, or unrelated files. A prior version of this repository held
the full vendor SDK release. This version keeps only the driver.

## Used by

[thingino-firmware](https://github.com/themactep/thingino-firmware)'s
`wifi-atbm6441` package builds this driver for boards that use the
ATBM6441 chip, starting with the Jooan S7-U (Ingenic T23ZN).

## License

Proprietary. This is vendor driver source, not open-source software.
