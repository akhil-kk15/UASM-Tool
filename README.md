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

- **Compiler**: C++17 compliant compiler (GCC 9+ or Clang 10+)
- **Build System**: CMake 3.16 or higher
- **Framework**: Qt 6 (or Qt 5.15) with `Widgets` and `Sql` modules
- **Database**: SQLite3

---

## How to Compile

### 1. Configure with CMake

From the repository root directory:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
```

If Qt is installed in a custom location, specify the prefix path:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
```

### 2. Build the Application and Tests

```bash
cmake --build build -j$(nproc)
```

This compiles the main application executable `AIDS` and the test binary `test_rotation_uniform`.

---

## How to Run

### Run Main GUI Application

```bash
./build/AIDS
```

### Run Automated Tests

Using CTest:

```bash
ctest --test-dir build --output-on-failure
```

Or run the test binary directly:

```bash
./build/test_rotation_uniform
```

### Seed Test Database

To populate the database with test teams (including Game Show Host teams), roles, inventory, and employees:

```bash
sqlite3 uniformmanagement.db < seed_test_data.sql
cp uniformmanagement.db build/uniformmanagement.db
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
