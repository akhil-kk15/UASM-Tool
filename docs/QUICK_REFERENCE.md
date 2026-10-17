# Quick Reference Guide - UASM-Tool

## Getting Started in 5 Minutes

### 1. Open the Project
```bash
cd UASM-Tool
qtcreator CMakeLists.txt &
```

### 2. Configure
- Qt Creator appears → Click "Configure Project"
- Select your Kit → Build

### 3. Build & Run
- Press `Ctrl + B` (Build)
- Press `Ctrl + R` (Run)
- Application appears on screen

### 4. Add Your First Employee
1. Go to "Employees" tab
2. Click "Add Employee"
3. Fill in: Name, Designation, Department, Email, Phone
4. Select Uniform Size from dropdown
5. Click OK

## Common Tasks

### View/Manage Employees
| Task | Steps |
|------|-------|
| **View all employees** | Click "Employees" tab → See table |
| **Add employee** | Click "Add Employee" → Fill form → OK |
| **Edit employee** | Select row → Click "Edit Employee" → Modify → OK |
| **Delete employee** | Select row → Click "Delete Employee" → Confirm |
| **Refresh list** | Click "Refresh" button |

### View/Manage Uniforms
| Task | Steps |
|------|-------|
| **View inventory** | Click "Uniforms" tab → See table |
| **Add uniform** | Click "Add Uniform" → Enter type, qty, size → OK |
| **Edit uniform** | Select row → Click "Edit Uniform" → Modify → OK |
| **Delete uniform** | Select row → Click "Delete Uniform" → Confirm |
| **Refresh list** | Click "Refresh" button |

### Track Issues/Returns
| Task | Steps |
|------|-------|
| **Issue uniform** | Click "Issue Uniform" → Select employee & uniform → OK |
| **Return uniform** | Select transaction → Click "Return Uniform" → Done |
| **View history** | Click "Issue/Return" tab → See all transactions |
| **Refresh list** | Click "Refresh" button |

## Qt Creator Shortcuts

| Shortcut | Action |
|----------|--------|
| `Ctrl + B` | Build project |
| `Ctrl + R` | Run project |
| `F5` | Debug project |
| `Ctrl + F` | Find in files |
| `Ctrl + /` | Toggle comment |
| `F1` | Show help |

## Editing in Qt Designer

### Open a UI File
1. Project panel (left) → Forms folder
2. Double-click `.ui` file
3. Qt Designer opens

### Make Changes
1. Drag widgets from left panel
2. Arrange in design area
3. Right-click for properties
4. Save with `Ctrl + S`

### Changes Auto-Compile
- Save `.ui` file
- Qt auto-generates header
- Build with `Ctrl + B`

## Database File

- **Location**: `build/` folder
- **Filename**: `uniformmanagement.db`
- **Format**: SQLite
- **Auto-created**: First run
- **Reset**: Delete file, restart app

## File Quick Reference

| File | Purpose |
|------|---------|
| `CMakeLists.txt` | Build configuration |
| `main.cpp` | App entry point |
| `mainwindow.*` | Main window |
| `*dialog.*` | Form dialogs |
| `employee.h/cpp` | Employee model |
| `uniform.h/cpp` | Uniform model |
| `uniformissue.h/cpp` | Issue model |
| `databasemanager.*` | Database operations |
| `*.ui` | UI designs (Qt Designer) |

## Uniform Sizes Available
- XS (Extra Small)
- S (Small)
- M (Medium)
- L (Large)
- XL (Extra Large)
- XXL (Extra Extra Large)

## Issue Status Values
- **Issued**: Currently with employee
- **Returned**: Returned to inventory
- **Pending**: Awaiting action

## Data Validation

### Employee Form
- Name: Required, text
- Email: Text format
- Phone: 10 digits recommended
- Size: Select from dropdown

### Uniform Form
- Type: Required (e.g., "Shirt", "Pants")
- Quantity: Number > 0
- Date: Calendar picker

### Issue Form
- Employee: Required, dropdown
- Uniform: Required, dropdown
- Quantity: Number > 0
- Date: Auto-filled with today

## Error Messages

| Error | Solution |
|-------|----------|
| "Database not found" | Delete `build/`, rebuild |
| "Kit not configured" | Select kit in bottom-left, reconfigure |
| "UI not loading" | Clean all, rebuild with `Ctrl + B` |
| "Field required" | All required fields must be filled |

## Column Meanings

### Employees Table
- **ID**: Unique employee number
- **Name**: Full employee name
- **Designation**: Job title
- **Department**: Department name
- **Email**: Email address
- **Phone**: Contact phone
- **Uniform Size**: Assigned size
- **Joining Date**: Employment start date

### Uniforms Table
- **ID**: Inventory item ID
- **Type**: Uniform type (T-shirt, Pants, etc.)
- **Quantity**: Available in stock
- **Size**: Uniform size
- **Color**: Item color
- **Purchase Date**: When added to inventory
- **Description**: Additional info

### Issue/Return Table
- **ID**: Transaction ID
- **Employee**: Employee name
- **Uniform Type**: What was issued
- **Quantity**: How many
- **Issue Date**: When given
- **Return Date**: When returned (if returned)
- **Status**: Issued/Returned/Pending

## Building from Command Line

```bash
# Navigate to project
cd UASM-Tool

# Create build folder
mkdir -p build && cd build

# Configure (replace path with your Qt installation)
cmake -DCMAKE_PREFIX_PATH=/opt/Qt/6.x.x/gcc_64 ..

# Build
make -j$(nproc)

# Run
./UASM-Tool
```

## Troubleshooting Quick Fixes

### App won't start
1. Delete `build/` folder
2. Delete `uniformmanagement.db`
3. Clean: `Build → Clean All`
4. Rebuild: `Ctrl + B`

### UI changes not showing
1. Save `.ui` file: `Ctrl + S`
2. Clean build: `Build → Clean All`
3. Rebuild: `Ctrl + B`

### Database errors
1. Look at output panel (bottom)
2. Check file permissions
3. Delete database, restart app

### Compilation errors
1. Check all `.ui` files are saved
2. Verify Qt installation path
3. Check CMake version ≥ 3.16

## Tips & Tricks

1. **Table navigation**: Click column header to sort (if implemented)
2. **Quick clear**: Select all → Delete to clear unused entries
3. **Backup data**: Copy `uniformmanagement.db` to backup
4. **UI customization**: All `.ui` files editable in Designer
5. **Fast build**: Close other Qt Creator windows

## Common File Edits

### To add a new field to Employee form:
1. Open `employeededialog.ui` in Designer
2. Drag QLineEdit from left panel
3. Drag QLabel, name it appropriately
4. Save file
5. In `employeedialog.cpp`, add getter/setter

### To add a new column in table:
1. Open `mainwindow.cpp`
2. Find `loadEmployeesTable()` function
3. Add `ui->employeesTable->setItem(...)` line
4. Update column count in `.ui` file

### To change database location:
1. Edit `databasemanager.cpp`
2. Find `m_database.setDatabaseName(...)`
3. Change path to desired location

## Useful Qt Documentation

While in Qt Creator, press `F1` or:
- Check Help menu
- Search for "QTableWidget" to customize tables
- Search for "QDialog" to understand dialog pattern
- Search for "QSqlDatabase" for database info

## Contact & Support

For extended functionality:
- Refer to `ARCHITECTURE.md` for technical details
- Check `README.md` for full features

## Version Information

- **Project**: UASM-Tool v1.0.0
- **Qt Support**: Qt 5.15 and Qt 6.x
- **C++ Standard**: C++17
- **Database**: SQLite 3
- **CMake**: 3.16+

---

**Ready to use! Questions? Check the README.md or ARCHITECTURE.md files.**
