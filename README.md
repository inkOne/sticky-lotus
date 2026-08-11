# 🌸 Sticky Lotus

> A lightweight Magic: The Gathering life counter built for the Seeed Studio reTerminal E1001 / Sticky.

<p align="center">
  <img src="docs/images/sticky-lotus-logo.png" alt="Sticky Lotus" width="500">
</p>

<p align="center">
  A fast, touch-first life counter for Magic: The Gathering — designed specifically
  for the E-Ink display of the Seeed Studio Sticky.
</p>

---

## About Sticky Lotus

**Sticky Lotus** turns the Seeed Studio Sticky into a dedicated tabletop companion
for Magic: The Gathering.

The project focuses on the things you actually need during a game:

- Life tracking for 2 or 4 players
- Commander Damage
- Poison Counters
- Touch gestures
- Long-press ±10 life changes
- Configurable starting life
- Battery status
- Persistent game state
- Deep Sleep support
- E-Ink optimized partial refreshes

The goal is a device that feels less like an embedded development board
and more like a purpose-built Magic accessory.

---

## Hardware

Sticky Lotus is designed for the **Seeed Studio Sticky**.

👉 [Seeed Studio Sticky](SEEED-STICKY-URL)

The device combines an ESP32-S3, touch input and an E-Ink display,
making it particularly well suited for a Magic: The Gathering life counter:
the display remains readable without a backlight and even keeps its
last image while the device is sleeping.

### Seeed Firmware Hub / Firmware Flasher

Seeed Studio provides a browser-based firmware flasher for the reTerminal
E-Series.

It can be used to:

- try official demo firmware
- restore an official firmware image
- test the device without setting up ESP-IDF
- recover the Sticky after experimenting with custom firmware

The Firmware Hub can be opened through the official reTerminal E1001
documentation:

[Open the reTerminal E1001 documentation and Firmware Flasher](https://wiki.seeedstudio.com/getting_started_with_reterminal_e1001/)

Connect the Sticky via USB-C, select the device in the browser and choose
the firmware that should be flashed.

This is also a useful recovery path if you want to return from Sticky Lotus
to the official Seeed Studio firmware.


## Upstream Firmware & Credits

Sticky Lotus was not developed completely from scratch.

A major source of inspiration and an invaluable reference during development was the [folloup-sticky project](https://github.com/alxv2016/folloup-sticky) by alxv2016.

I am incredibly grateful for the work that went into this project. It already demonstrated and implemented many of the fundamental building blocks needed to develop applications for the Seeed Studio Sticky — at a time when official documentation and source code for the device were not yet publicly available.

Being able to study a working implementation of the hardware integration made it significantly easier to understand the display, touch input, power management, battery handling, and other parts of the Sticky platform.

Thank you for making this work publicly available and giving others a foundation to learn from and build upon.

Sticky Lotus uses these insights as a starting point while implementing its own application architecture, user interface, game logic, and Magic: The Gathering–focused experience.
---

## Features

### Life Counter

Sticky Lotus supports both:

**2 Player**

and

**4 Player / Commander**

Each player receives an independent life counter with touch controls.

Tap the `+` or `−` area to change life by one.

Hold the area to change life by ten.

### Commander Damage

Swipe from a player area to open the Commander Damage interface.

Commander damage can be edited independently for each attacking player
and is applied when leaving the Commander Damage screen.

### Poison Counters

Poison counters are available directly through a swipe gesture from
the corresponding player area.

### Touch optimized

The interface is designed around the physical orientation of players
around a table.

Player areas facing the opposite side are rotated automatically so that
every player can read and operate their own controls.

### E-Ink optimized rendering

Instead of refreshing the entire screen after every input, Sticky Lotus
updates only the affected regions whenever possible.

This significantly improves perceived responsiveness and reduces
unnecessary E-Ink refreshes.

---

## Deep Sleep

Pressing the hardware power button puts Sticky Lotus into Deep Sleep.

Before sleeping:

1. The current game state is stored in NVS.
2. The Deep Sleep artwork is rendered.
3. The E-Ink display performs its final refresh.
4. The ESP32 enters Deep Sleep.

The E-Ink panel retains the sleep artwork without requiring continuous power.

Press the power button again to wake the device and restore the previous game.

---

## Simulator

You don't need a Sticky to work on the UI.

Sticky Lotus contains a desktop simulator that uses the same application
and rendering logic as the embedded firmware.

### Build

```bash
nix develop
idf.py build
idf.py -p PORT flash 
