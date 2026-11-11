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
ATBM6441 chip, starting with the Jooan S7-U (Ingenic T23ZN),
also tested on T41ZN.

## Unified Ingenic platform support

The newer version of this driver includes generic support for all
of the Ingenic Txx SoC variants, without having to declare any
further specific SoC definitions.

You only need to select PLATFORM_INGENIC as the target platform,
which is done by default in the Makefile.

The kernel module doesn't try to detect mmc1/mmc2 by default
any more, as this can be done using the userland mmc command-line tool.

If you still need to run mmc_detect in this kernel module, define one of
the following config options:

CONFIG_ATBM_WIFI_JZ_DETECT_MMC1_APPOLO_SDIO - previously tested on T31
CONFIG_ATBM_WIFI_JZ_DETECT_MMC1_ATBM_PLATFORM - previously tested on T41

## License

Proprietary. This is vendor driver source, not open-source software.
