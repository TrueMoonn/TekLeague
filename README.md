# TekLeague

**TekLeague** is a multiplayer game project inspired by *League of Legends*.
It is built in **C++** using a custom engine called **TrueEngine**, with a clear separation between **client** and **server**.

The project focuses on core MOBA mechanics such as champions, abilities, minions, items, stats, and real-time networking.

---

## Features

* Client / Server architecture
* Custom game engine (TrueEngine)
* Entity-component-system (ECS) based gameplay
* Champions, spells, items, and stats
* Minions, combat systems, and abilities
* Plugin-based engine extensions
* Cross-platform support (Linux / Windows)

---

## Project Structure

* `client/` – Game client (graphics, UI, player input)
* `server/` – Game server (game logic, networking, lobby)
* `src/` – Shared game logic
* `include/` – Shared headers
* `TrueEngine/` – Custom engine used by the project
* `plugins/` – Engine plugins (physics, interaction, display, etc.)
* `build/` – Build directory (generated)

---

## Build Instructions

The project uses **CMake** and a helper build script.

### Build the project

```bash
./build.sh --build
```

### Rebuild from scratch

```bash
./build.sh --re-build
```

### Debug build

```bash
./build.sh --debug-build
```

### Clean build files

```bash
./build.sh --clear
```

## Inspiration

TekLeague is inspired by **League of Legends**, but is developed as an educational and technical project to explore:

* Game engine architecture
* Multiplayer networking
* ECS design
* Modular plugins


## Disclaimer

This project is **not affiliated with Rito Games**.
League of Legends is a registered trademark of Rito Games, Inc.
