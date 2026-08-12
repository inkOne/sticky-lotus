# Sticky Lotus

> A lightweight Magic: The Gathering life counter built for the Seeed Studio reTerminalSticky.

<p align="center">
  <img src="assets/images/sticky-lotus.jpeg" alt="Sticky Lotus" width="500">
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

**Swipe left or right** from a player area to open the Commander Damage interface.

Commander damage can be edited independently for each attacking player
and is applied when leaving the Commander Damage screen.

### Poison Counters

Poison counters are available directly through a **swipe up** gesture from
the corresponding player area and **swipe down** to close. 

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

The simulator hasn’t been used since the hardware arrived and will probably need a little love before it works with the current version of the software again.


# Release 

https://github.com/inkOne/sticky-lotus/tree/main/firmware/release

## Build

```bash
nix develop
idf.py build
idf.py -p PORT flash 
```
#### Merged bin
 ```
 idf.py merge-bin
  ```

`merged-binary.bin` can be updoadet to https://www.seeedstudio.com/sticky/playground/official-firmware/



## Upstream Firmware & Credits

Sticky Lotus was not developed completely from scratch.

A major source of inspiration and an invaluable reference during development was the [folloup-sticky project](https://github.com/alxv2016/folloup-sticky) by alxv2016.

I am incredibly grateful for the work that went into this project. It already demonstrated and implemented many of the fundamental building blocks needed to develop applications for the Seeed Studio Sticky — at a time when official documentation and source code for the device were not yet publicly available.

Being able to study a working implementation of the hardware integration made it significantly easier to understand the display, touch input, power management, battery handling, and other parts of the Sticky platform.

Thank you for making this work publicly available and giving others a foundation to learn from and build upon.

Sticky Lotus uses these insights as a starting point while implementing its own application architecture, user interface, game logic, and Magic: The Gathering–focused experience.
---
