## Cellular Automata

An interactive, high-performance 2D cellular automata simulator written in C++ with SDL3. It lets you experiment with custom `B/S` rules (e.g. Conway’s Game of Life `B3S23`), draw patterns in real time, and visualize how simple local rules produce rich global behavior.


## Demo / What It Does

- **2D grid simulation** where cells evolve over time based on configurable rules.
- **Draw and erase cells** with the mouse while the simulation is running.
- **Custom Life-like rules** entered via console input:
  - Example: `B3S23` → Conway’s Game of Life.
- **Visual customization**:
  - Choose alive cell color: red, green, or blue.
  - Optional **fog effect**: dead cells fade based on how long ago they were alive.

## Technologies Used

- **Language**: C++
- **Graphics / Windowing**: SDL3 (`<SDL3/SDL.h>`)
- **IDE / Build**: Visual Studio C++ project (`.sln`, `.vcxproj`)

---

## Installation

### 1. Prerequisites

- **OS**: Windows
- **Tools**: Visual Studio with **Desktop development with C++** workload.
- **Libraries**: SDL3 development libraries installed and configured.

### 2. Clone and Open

```bash
git clone https://github.com/Vladbori21/Cellular-Automata.git
cd Cellular-Automata
```

- Open `Cellular Automata.sln` in Visual Studio.

### 3. Configure SDL3

In project properties:

- **C/C++ → General → Additional Include Directories**: add SDL3 `include`.
- **Linker → General → Additional Library Directories**: add SDL3 `lib`.
- **Linker → Input → Additional Dependencies**: ensure `SDL3.lib` (or equivalent).
- Make sure the SDL3 DLL is reachable at runtime (next to the exe or on `PATH`).

### 4. Build

- Choose `Debug` or `Release`.
- `Build` → `Build Solution`.

---

## Usage

### Launch

- Run from Visual Studio (`F5` / `Ctrl+F5`) or execute the built binary.

### Initial Console Prompts

1. **Rule**

   ```text
   Enter rule:
   B-amount of neighbors where the cell born
   S-amount of neighbors where the cell survives
   Example: Conway's Game of Life is B3S23
   ```

   Example input:

   ```text
   B3S23
   ```

2. **Color**

   ```text
   Enter color: 1-Red 2-Green 3-Blue
   ```

3. **Fog**

   ```text
   Fog effect: 1-Yes 2-No
   ```

### Controls (Window)

- **Mouse**
  - Left click: set cell **alive**.
  - Right click: set cell **dead**.
- **Keyboard**
  - `Space`: pause / resume simulation.
  - `C`: clear the grid.
- Close the window to exit.

---

## Project Structure

- `Cellular Automata/src/main.cpp`
  Entry point; constructs `Game` and calls `run()`.
- `Cellular Automata/src/Game.h`
  Core types and interfaces:
  - `Screen` constants (window size, cell size, grid dimensions).
  - `Color`, `Cell`, `Rule`.
  - `Grid` and `Game` class declarations.
- `Cellular Automata/src/Game.cpp`
  Implementation of:
  - Grid update logic and neighbor counting.
  - SDL3 setup, input handling, update loop, and rendering.


## Configuration and Parameters

- **Grid / Screen (compile-time)**:
  - `WINDOW_WIDTH`, `WINDOW_HEIGHT`
  - `LOGICAL_WIDTH`, `LOGICAL_HEIGHT`
  - `GRID_SIZE`
  - `GRID_WIDTH`, `GRID_HEIGHT`
- **Simulation rate (compile-time)**:
  - `frameRate = 5;`
  - `frameDuration = 1000.0f / frameRate;`
- **Runtime configuration (via console)**:
  - Rule string (`B...S...`).
  - Color (`1`–`3`).
  - Fog (`1`/`2`).

To tweak grid density or speed, adjust values in `Game.h` and rebuild.

---

## Architecture Overview

- **`Grid`**
  - Owns the 2D grid and rule.
  - Responsible for neighbor counting and applying the rule each step.
- **`Game`**
  - Owns SDL window and renderer.
  - Manages:
    - Initialization and cleanup.
    - User configuration and runtime input.
    - Main loop: `handleInput` → `update` → `render`.
- **`main`**
  - Minimal bootstrap; delegates to `Game`.

This separation keeps the simulation logic testable and independent from SDL specifics, while the `Game` class encapsulates the real-time application concerns.
