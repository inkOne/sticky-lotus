# Sticky hardware port

This port reuses the existing `core` and `engine` sources directly inside the
ESP-IDF build. No duplicate game logic or separate hardware UI was introduced.

## New adapter classes

- `platform/sticky/StickyCanvas`: writes the existing Canvas drawing commands
  into the EpaperPanel 1-bit framebuffer.
- `platform/sticky/StickyImageRenderer`: embeds the Poison icon for firmware.
- `platform/sticky/StickyInputProvider`: reads the two side buttons. Touch is
  intentionally not wired yet because the touch-controller component is not
  part of the supplied firmware components.

## Build

From the firmware Nix shell:

```sh
cd firmware
rm -rf build
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/cu.usbmodem5C850491601 flash monitor
```

The first firmware run draws the same initial Game screen as the simulator and
performs one full E-paper refresh. It deliberately does not refresh in a loop.
