# UASM-Tool - Utilities and Schedule Management Tool

A Qt6 / C++ desktop application designed to manage employee uniforms, extra office inventory, team staffing, and 8-hour shift table rotations with cyclical breaks.

---

## Features

- **Uniform Inventory Management**: Tracks uniform types, sizes (`XS` through `XXL`), colors, total office stock, and real-time extra available uniforms (`Available = Total Stock - Currently Issued`).
- **Dealer Roles & Dedicated Teams**:
  - Employees have role types: **Dealer**, **Game Show Host**, or **Both**.
  - Teams are classified as **Standard** (dealers) or **Game Show Host** (hosts).
  - Enforces role compatibility: Game Show Host teams only admit Game Show Hosts or dual-role employees, while Standard teams require Dealer or dual-role staff.
- **Uniform-Verified Reassignment & Role-Safe Fallback**:
  - Validates role compatibility and checks uniform inventory in the employee's size prior to reassignment.
  - If uniforms are out of stock, automatically searches and redirects to a compatible alternative team with available uniform stock and staffing capacity.
- **Table Rotation Engine (30 Mins & 40 Mins)**:
  - Supports configurable rotation types: **30 Mins** and **40 Mins**.
  - Generates full shift schedules where each employee attends 16 tables across 4 rounds.
  - In each round, an employee works 4 consecutive tables followed by the 5th slot as a 30-minute break.
  - Automatically recalculates and adjusts rotations when team membership changes.
- **Issue & Return Tracking**: Maintains complete transaction history for uniform allocations and returns.

---

## Prerequisites

- **Compiler**: C++17 compliant compiler (MSVC 2019/2022, MinGW-w64 GCC 9+, GCC 9+, or Clang 10+)
- **Build System**: CMake 3.16 or higher
- **Framework**: Qt 6 (or Qt 5.15) with `Widgets` and `Sql` modules (including the `qsqlite` SQLite driver plugin)
- **Database**: SQLite3

---

## How to Compile

A `CMakePresets.json` is included, so Visual Studio (Windows) and VS Code / Qt Creator (Linux) can configure the project with the right settings and no manual Qt paths (requires CMake 3.21+; recent VS 2022 releases bundle a newer CMake). Qt is auto-detected from the standard install locations - `C:/Qt`, `D:/Qt`, `%USERPROFILE%/Qt` on Windows; `~/Qt` and `/opt/Qt` on Linux - or from the `QTDIR` environment variable. An explicit `-DCMAKE_PREFIX_PATH=...` always wins.

### Visual Studio 2022 (Windows)

1. **File → Open → Folder...** and select the repository root.
2. If Visual Studio asks which configuration source to use, choose **CMakePresets.json**, then select the **`windows-msvc`** preset.
3. Build with **Ctrl+Shift+B**, run/debug with **F5**.

The debugger environment and working directory are configured automatically, and after the first build the required Qt DLLs and plugins are deployed next to the executable, so the app also starts from Explorer or any terminal - no manual `windeployqt` step. (Disable the automatic deployment with `-DUASM_DEPLOY_QT=OFF` if you prefer to manage the runtime yourself.)

> **Important - use the MSVC build of Qt.** Visual Studio compiles with MSVC, which cannot link against a MinGW Qt. If your Qt installation only has the MinGW kit (for example `D:\Qt\6.10.0\mingw_64`), add the MSVC kit once with the **Qt Maintenance Tool**: *6.10.0 → MSVC 2022 64-bit*. Then re-configure and Qt is found automatically. If Qt is installed somewhere unusual, set a `QTDIR` environment variable (for example `D:\Qt\6.10.0\msvc2022_64`).

### VS Code (Linux)

The CMake Tools extension reads the same `CMakePresets.json`: select the **`linux-debug`** preset, or from a terminal:

```bash
cmake --preset linux-debug
cmake --build --preset linux-debug
ctest --preset linux-debug
```

### Windows (MinGW) - works with the MinGW kit that ships with the Qt installer

The **`windows-mingw`** preset builds with Qt's bundled MinGW and Ninja (paths pinned to the standard `D:\Qt` layout in `CMakePresets.json`; adjust them if your install differs):

```powershell
cmake --preset windows-mingw
cmake --build --preset windows-mingw
ctest --preset windows-mingw
.\build\windows-mingw\UASM-Tool.exe
```

The Qt and MinGW runtime DLLs are deployed next to the executable automatically on Windows, so no PATH setup is needed to run the tests or the application.

The command-line recipes below work as well; the presets are simply the shortest path.

### Linux / macOS

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

If Qt is installed in a custom directory, pass `-DCMAKE_PREFIX_PATH`:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
cmake --build build -j$(nproc)
```

### Windows (MSVC / Visual Studio)

Open a **Developer PowerShell for VS** (or standard PowerShell with CMake installed) from the repository root:

```powershell
cmake -B build -S . -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64"
cmake --build build --config Release
```

The required Qt DLLs, plugins (including the SQLite driver `qsqlite.dll`) and the compiler runtime are deployed next to the executable automatically after building, so no `windeployqt` call is needed. To run it manually anyway:

```powershell
C:/Qt/6.x.x/msvc2019_64/bin/windeployqt.exe build/Release/UASM-Tool.exe
```

### Windows (MinGW)

```powershell
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
cmake --build build
```

The Qt runtime and the MinGW runtime DLLs are deployed automatically after building (manual fallback: `C:/Qt/6.x.x/mingw_64/bin/windeployqt.exe --release build/UASM-Tool.exe`).

---

## How to Run

### Run Main GUI Application

- **Linux / macOS**:
  ```bash
  ./build/UASM-Tool
  ```
- **Windows (MSVC)**:
  ```powershell
  .\build\Release\UASM-Tool.exe
  ```
- **Windows (MinGW)**:
  ```powershell
  .\build\UASM-Tool.exe
  ```

### Run Automated Tests

- **Linux / macOS**:
  ```bash
  ctest --test-dir build --output-on-failure
  ```
- **Windows (MSVC)**:
  ```powershell
  ctest --test-dir build -C Release --output-on-failure
  ```
- **Windows (MinGW)**:
  ```powershell
  ctest --test-dir build --output-on-failure
  ```

---

## Database Setup & Sample Data

A pre-populated sample database `uniformmanagement.db` is included directly in the repository root and is staged into the target executable directory the first time you build (the CMake build copies it only when missing, so rebuilding never overwrites data the application has written; delete the copy next to the executable to re-stage the pristine sample). It contains:
- 5 Teams: 4 Standard Dealer teams and 1 dedicated Game Show Host team (`Studio Live`).
- 24 Employees: configured with `Dealer`, `Game Show Host`, and `Both` role types.
- Complete Uniform Inventory: across sizes (`S`, `M`, `L`, `XL`), colors, and teams (including intentional out-of-stock scenarios to demonstrate uniform-checked fallback redirection).
- Transactional records for uniform allocations and returns.

### How to Populate or Reset the Database

**Linux / macOS**:

```bash
sqlite3 uniformmanagement.db < seed_test_data.sql
cp uniformmanagement.db build/uniformmanagement.db
```

**Windows (PowerShell)**:

```powershell
Get-Content seed_test_data.sql | sqlite3.exe uniformmanagement.db
Copy-Item uniformmanagement.db -Destination build\Release\uniformmanagement.db -Force
```

**Windows (Command Prompt / CMD)**:

```cmd
sqlite3.exe uniformmanagement.db < seed_test_data.sql
copy /Y uniformmanagement.db build\Release\uniformmanagement.db
```

### How to Verify Database Records

Inspect the populated records using the `sqlite3` CLI:

```bash
sqlite3 uniformmanagement.db "SELECT id, name, team_type, uniform_type FROM teams;"
sqlite3 uniformmanagement.db "SELECT id, name, department, role_type, uniform_size FROM employees LIMIT 10;"
sqlite3 uniformmanagement.db "SELECT id, type, team, size, quantity FROM uniforms;"
```

---

## Project Structure

```
.
├── CMakeLists.txt            # CMake build configuration
├── seed_test_data.sql        # SQL seed script with test teams & roles
├── src/
│   ├── main.cpp              # Application entry point
│   ├── mainwindow.h/.cpp     # Main window with tabbed interface
│   ├── employee.h/.cpp       # Employee model (roles, size, team)
│   ├── team.h/.cpp           # Team model (team type, 4 tables, staffing)
│   ├── uniform.h/.cpp        # Uniform model (type, team, stock, size)
│   ├── uniformissue.h/.cpp   # Issue & return transaction model
│   ├── rotationscheduler.h/.cpp # Rotation generator (30m/40m) & reassign fallback
│   ├── databasemanager.h/.cpp# SQLite database operations
│   ├── employeedialog.h/.cpp # Dialog for adding/editing employees & roles
│   ├── uniformdialog.h/.cpp  # Dialog for uniform inventory
│   ├── issuedialog.h/.cpp    # Dialog for issuing uniforms
│   └── reassigndialog.h/.cpp # Dialog for role- and uniform-verified reassignment
├── ui/                       # Qt Designer UI layouts (.ui)
└── tests/
    └── test_rotation_uniform.cpp # Automated test suite
```
