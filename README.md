# AIDS - Uniform & Shift Management System

A Qt6 / C++ desktop application designed to manage employee uniforms, extra office uniform inventory, team staffing, and 8-hour shift table rotation schedules with cyclical breaks.

---

## Features

- **Uniform Inventory Management**: Tracks uniform types, sizes (`XS` through `XXL`), colors, total office stock, and real-time extra available uniforms (`Available = Total Stock - Currently Issued`).
- **Team Staffing & 4-Table Coverage**:
  - Each team is assigned 4 tables (Tables 1 through 4).
  - Evaluates staffing thresholds (`Critically Understaffed` < 4, `Understaffed` < target, `Optimal` = target, `Overstaffed` > target).
- **Uniform-Verified Reassignment & Auto-Fallback**:
  - Checks uniform stock in the employee's size before moving them to a target team.
  - If uniform is out of stock, automatically redirects the employee to an alternative team with available stock and staffing capacity.
  - One-click **Auto-Balance Staff** to rebalance teams respecting uniform inventory.
- **8-Hour Shift Table Rotation Engine**:
  - Computes complete 8-hour shift schedules (16 slots at 30-minute intervals or 8 slots at 60-minute intervals).
  - Ensures all 4 tables remain staffed at every work slot.
  - Enforces that after attending 4 tables, employees receive a **30-minute break**, repeating cyclically.
  - Automatically recalculates and adjusts rotations when team membership changes.
- **Issue & Return Tracking**: Maintains complete history of issued and returned uniforms and accessories.

---

## Prerequisites

- **Compiler**: C++17 compliant compiler (GCC 9+ or Clang 10+)
- **Build System**: CMake 3.16 or higher
- **Framework**: Qt 6 (or Qt 5.15) with `Widgets` and `Sql` modules
- **Database**: SQLite3 (bundled / Qt SQLite driver)

---

## How to Compile

### 1. Configure with CMake

From the repository root directory:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
```

*Note: If Qt is installed in a non-standard location, provide the Qt prefix path flag:*

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/opt/Qt/6.8.0/gcc_64
```

### 2. Build the Application and Tests

```bash
cmake --build build -j$(nproc)
```

This compiles both the main application executable `AIDS` and the verification test suite `test_rotation_uniform`.

---

## How to Run

### Run Main GUI Application

```bash
./build/AIDS
```

### Run Automated Tests

```bash
./build/test_rotation_uniform
```

---

## Project Structure

```
.
├── CMakeLists.txt            # CMake build configuration
├── src/
│   ├── main.cpp              # Application entry point
│   ├── mainwindow.h/.cpp     # Main window with tab interfaces
│   ├── employee.h/.cpp       # Employee model (size, team, designation)
│   ├── uniform.h/.cpp        # Uniform model (type, team, stock, size)
│   ├── uniformissue.h/.cpp   # Issue & return transaction model
│   ├── team.h/.cpp           # Team model (4 tables, staffing status)
│   ├── rotationscheduler.h/.cpp # 8h shift rotation & fallback engine
│   ├── databasemanager.h/.cpp# SQLite database operations & queries
│   ├── employeedialog.h/.cpp # Dialog for adding/editing employees
│   ├── uniformdialog.h/.cpp  # Dialog for managing uniform inventory
│   ├── issuedialog.h/.cpp    # Dialog for issuing uniforms
│   └── reassigndialog.h/.cpp # Dialog for uniform-checked reassignment
├── ui/                       # Qt Designer UI layouts (.ui)
├── tests/
│   └── test_rotation_uniform.cpp # Automated test suite
└── docs/                     # Documentation files
```
