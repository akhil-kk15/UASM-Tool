# AIDS - Uniform & Shift Management System

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

To package the required Qt DLLs and the SQLite driver (`qsqlite.dll`) into the executable directory on Windows, run `windeployqt`:

```powershell
C:/Qt/6.x.x/msvc2019_64/bin/windeployqt.exe build/Release/AIDS.exe
```

### Windows (MinGW)

```powershell
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
cmake --build build
C:/Qt/6.x.x/mingw_64/bin/windeployqt.exe build/AIDS.exe
```

---

## How to Run

### Run Main GUI Application

- **Linux / macOS**:
  ```bash
  ./build/AIDS
  ```
- **Windows (MSVC)**:
  ```powershell
  .\build\Release\AIDS.exe
  ```
- **Windows (MinGW)**:
  ```powershell
  .\build\AIDS.exe
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

A pre-populated sample database `uniformmanagement.db` is included directly in the repository root and is automatically staged into the target executable directory during the CMake build. It contains:
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
