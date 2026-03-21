# UT-Godot-Engine

<div align="center">
  <img src="./image/UT-Godot-Engine_logo.gif" alt="UT-Godot-Engine Logo" width="500"/>
</div>

<div align="center">

[![Godot 4.5](https://img.shields.io/badge/Godot-4.5-478cbf?logo=godot-engine&logoColor=white)](https://godotengine.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

</div>

## Overview

**UT-Godot-Engine** is an **Undertale-style fan game framework** built on **Godot Engine** with **C++ (GDExtension)**. It focuses on rapid setup for battle flow, dialogue presentation, and overworld event logic, while keeping the project structure clean and modular.

## Highlights

- Turn-based battle system inspired by Undertale
- Dialogue typing and text presentation tools
- Overworld transitions and event triggers
- Modular systems and user-friendly APIs

## Getting Started

1. Download the latest release.
2. Open the project with Godot Engine 4.5+.
3. Explore the sample scenes and start building your game.

## Docs and References

- Tutorial site: https://wheedo07.github.io/UT-Godot-Engine/
- Godot class reference: https://docs.godotengine.org/

## Folder Structure & Rules

Each folder is color-coded in the Godot editor to indicate how freely it can be modified.

| Color | Folders | Rules |
|-------|---------|-------|
| 🔴 Red | `res://Engine/`, `res://Core/DefaultScene/`, `res://bin/` | **Do not touch.** Requires a complete understanding of engine internals. Avoid unless absolutely necessary. |
| 🟠 Orange | `res://Core/` (includes `autoload/`) | Scenes and resources inside may be edited, but: root nodes cannot have scripts attached, existing scenes/resources must not be deleted, adding nodes is allowed. |
| 🟡 Yellow | `res://Core/Custom/`, `res://Core/Startup/` | Freely editable — just don't delete existing files. |

> For a full breakdown, see the [Project Structure](https://wheedo07.github.io/UT-Godot-Engine/en/index.html#/tutorials/project-structure) tutorial in the docs.

## Tech Stack

- Godot Engine 4.5
- GDExtension (C++17)

## Credits

- [@wheedo07](https://github.com/wheedo07) - project lead and development
- [@chqh](https://github.com/chqh0311) - engine logo, feedback, and testing
- Toby Fox - Original Undertale creator

## License

Released under the MIT License. See [`LICENSE`](LICENSE) for details.