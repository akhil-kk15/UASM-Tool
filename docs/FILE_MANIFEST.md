# Project File Manifest - AIDS

## Complete File Listing

### Core Application Files

#### **Application Entry Point**
- **`main.cpp`** - Application initialization and Qt event loop start

#### **Main Window (UI + Logic)**
- **`mainwindow.h`** - Main window class declaration with slots
- **`mainwindow.cpp`** - Main window implementation with table loading and button handlers
- **`mainwindow.ui`** - Main window design (XML, editable in Qt Designer)
  - 3 tabs: Employees, Uniforms, Issue/Return
  - Tables for data display
  - Action buttons

#### **Dialog Windows**

**Employee Dialog**
- **`employeedialog.h`** - Employee form dialog class
- **`employeedialog.cpp`** - Employee form implementation
- **`employeededialog.ui`** - Employee form design (XML)

**Uniform Dialog**
- **`uniformdialog.h`** - Uniform form dialog class
- **`uniformdialog.cpp`** - Uniform form implementation
- **`uniformdialog.ui`** - Uniform form design (XML)

**Issue Dialog**
- **`issuedialog.h`** - Issue/return dialog class
- **`issuedialog.cpp`** - Issue/return form implementation
- **`issuedialog.ui`** - Issue/return form design (XML)

### Data Model Classes

#### **Employee Model**
- **`employee.h`** - Employee class declaration
  - Properties: id, name, designation, department, email, phone, uniformSize, joiningDate, active
  - Getters and setters for all properties
- **`employee.cpp`** - Employee class implementation

#### **Uniform Model**
- **`uniform.h`** - Uniform class declaration
  - Properties: id, type, quantity, size, color, purchaseDate, cost, description
  - Getters and setters
- **`uniform.cpp`** - Uniform class implementation

#### **Uniform Issue Model**
- **`uniformissue.h`** - UniformIssue class declaration
  - Properties: id, employeeId, uniformId, quantity, issueDate, returnDate, status
  - Getters and setters
- **`uniformissue.cpp`** - UniformIssue class implementation

### Database Layer

#### **Database Manager**
- **`databasemanager.h`** - Database manager class declaration
  - Methods for employee CRUD operations
  - Methods for uniform CRUD operations
  - Methods for issue/return operations
  - Database initialization and table creation
  - Error handling
- **`databasemanager.cpp`** - Database manager implementation
  - SQLite database connection
  - SQL queries for all operations
  - Table schema definitions

### Build Configuration

- **`CMakeLists.txt`** - CMake build configuration
  - Qt 5/6 detection
  - Source file listing
  - Compiler settings
  - Project properties

### Database

- **`uniformmanagement.db`** - SQLite database file
  - Auto-created on first run
  - Location: build/ directory
  - Contains 3 tables: employees, uniforms, uniform_issues

### Documentation

#### **Quick Start & Reference**
- **`README.md`** - Main project documentation
  - Feature overview
  - Architecture description
  - Database schema
  - Building instructions
  - Usage guide
  - Future enhancements

#### **Qt Creator Integration Guide**
- **`QT_CREATOR_SETUP.md`** - Detailed Qt Creator instructions
  - How to open the project
  - Configuring kits
  - Building and running
  - Using Qt Designer
  - Common tasks
  - Keyboard shortcuts
  - Troubleshooting

#### **Technical Architecture**
- **`ARCHITECTURE.md`** - Detailed technical documentation
  - System architecture overview
  - Class diagrams
  - Data flow diagrams
  - Database relationships
  - Design patterns used
  - UI layout descriptions
  - Error handling strategy

#### **Delivery Summary**
- **`DELIVERY_SUMMARY.md`** - What has been delivered
  - Project overview
  - Feature checklist
  - Qt Creator integration details
  - Quick start guide
  - Next steps for customization

#### **Quick Reference**
- **`QUICK_REFERENCE.md`** - Quick reference guide
  - 5-minute quick start
  - Common tasks table
  - Keyboard shortcuts
  - Database info
  - Error solutions
  - File reference

#### **This File**
- **`FILE_MANIFEST.md`** - Complete file listing and descriptions

### Configuration & Metadata

#### **Git & Version Control**
- **`.gitignore`** - Git ignore rules (if version controlled)

#### **IDE Configuration**
- **`.qtcreator/`** - Qt Creator configuration folder
- **`.vscode/`** - VS Code settings (optional)

### Build Directory

- **`build/`** - Build artifacts folder (auto-generated)
  - Compiled objects
  - Executable
  - Build cache
  - CMake files
  - Database file (uniformmanagement.db)

#### **Old UI Backup** (from project init)
- **`mainwindow.ui.autosave`** - Qt Creator auto-save backup

---

## File Organization Summary

```
Project Root
│
├── Build Configuration
│   └── CMakeLists.txt
│
├── Application Source Code
│   ├── main.cpp
│   ├── mainwindow.h/cpp
│   ├── employeedialog.h/cpp
│   ├── uniformdialog.h/cpp
│   └── issuedialog.h/cpp
│
├── Data Models
│   ├── employee.h/cpp
│   ├── uniform.h/cpp
│   └── uniformissue.h/cpp
│
├── Database Layer
│   └── databasemanager.h/cpp
│
├── UI Design Files (Qt Designer)
│   ├── mainwindow.ui
│   ├── employeededialog.ui
│   ├── uniformdialog.ui
│   └── issuedialog.ui
│
├── Documentation
│   ├── README.md (Start here)
│   ├── QT_CREATOR_SETUP.md
│   ├── ARCHITECTURE.md
│   ├── DELIVERY_SUMMARY.md
│   ├── QUICK_REFERENCE.md
│   └── FILE_MANIFEST.md (this file)
│
├── Build Directory
│   ├── build/
│   └── uniformmanagement.db
│
└── Configuration
    ├── .gitignore
    ├── .qtcreator/
    └── .vscode/
```

## File Purpose Quick Reference

### Which file should I edit to...?

| Goal | File(s) | Type |
|------|---------|------|
| Add employee field | `employee.h/cpp`, `employeededialog.ui` | Code/UI |
| Change main window layout | `mainwindow.ui` | UI |
| Modify database operations | `databasemanager.cpp` | Code |
| Add new dialog | Create new `.h/.cpp/.ui` files | Code/UI |
| Fix compile error | Check `CMakeLists.txt`, `.h` files | Config/Code |
| Understand architecture | `ARCHITECTURE.md`, `README.md` | Docs |
| Learn Qt Creator | `QT_CREATOR_SETUP.md` | Docs |
| Quick reference | `QUICK_REFERENCE.md` | Docs |

## File Statistics

### Source Code Files: 9 pairs
- 9 header files (.h)
- 9 implementation files (.cpp)
- 4 UI design files (.ui)

### Total Lines (Approximate)
- Headers: ~400 lines
- Implementation: ~1200 lines
- UI files: ~500 lines
- CMakeLists.txt: ~80 lines

### Documentation
- 6 markdown files (~3000 total lines)

### Total Project Files: 31+
- Source code: 22
- Documentation: 8
- Configuration: 2
- Build artifacts: varies

## 🔄 File Dependencies

```
main.cpp
  ↓
  → mainwindow.h/cpp
      ↓
      → ui_mainwindow.h (generated from mainwindow.ui)
      → databasemanager.h/cpp
          ↓
          → employee.h/cpp
          → uniform.h/cpp
          → uniformissue.h/cpp
      → employeedialog.h/cpp
          ↓
          → ui_employeededialog.h (generated)
          → employee.h/cpp
      → uniformdialog.h/cpp
          ↓
          → ui_uniformdialog.h (generated)
          → uniform.h/cpp
      → issuedialog.h/cpp
          ↓
          → ui_issuedialog.h (generated)
          → employee.h/cpp
          → uniform.h/cpp
          → uniformissue.h/cpp
```

## Naming Conventions Used

### Header Files (.h)
- `ClassName.h` - Capital first letter

### Implementation Files (.cpp)
- `ClassName.cpp` - Matches header name

### UI Files (.ui)
- `classname.ui` - Lowercase (Qt convention)
- Note: employeededialog.ui has double 'd'

### Classes
- `ClassName` - PascalCase for class names
- `methodName()` - camelCase for methods
- `m_memberVariable` - 'm_' prefix for member variables

### Database Tables
- `employees` - lowercase, plural
- `uniforms` - lowercase, plural
- `uniform_issues` - lowercase with underscore

### Database Columns
- `employee_id` - snake_case for foreign keys
- `joining_date` - snake_case for dates
- `active` - boolean columns

## 🔐 File Permissions

- **Source files (.h/.cpp)**: Read/Write for editing
- **UI files (.ui)**: Read/Write (editable in Designer)
- **CMakeLists.txt**: Read/Write (build configuration)
- **uniformmanagement.db**: Read/Write (database)
- **Markdown files**: Read/Write (documentation)

## File Sizes (Approximate)

| File Type | Size |
|-----------|------|
| Header files | 2-4 KB each |
| Implementation files | 3-8 KB each |
| UI files | 2-4 KB each |
| CMakeLists.txt | 2 KB |
| Database | Grows with data |
| Documentation | Varies |

## Generated Files

These files are auto-generated and should NOT be edited:
- `ui_mainwindow.h` (from mainwindow.ui)
- `ui_employeededialog.h` (from employeededialog.ui)
- `ui_uniformdialog.h` (from uniformdialog.ui)
- `ui_issuedialog.h` (from issuedialog.ui)
- `moc_*.cpp` (Meta-Object Compiler output)
- Build artifacts in `build/` directory

## Documentation Reading Order

1. **First Time**: `README.md`
2. **Quick Questions**: `QUICK_REFERENCE.md`
3. **Understanding Design**: `ARCHITECTURE.md`
4. **Specific Tasks**: Respective documentation sections

## Getting Started Checklist

- [ ] Read `README.md`
- [ ] Read `QT_CREATOR_SETUP.md`
- [ ] Open project in Qt Creator
- [ ] Build project (Ctrl + B)
- [ ] Run application (Ctrl + R)
- [ ] Add test data
- [ ] Double-click `.ui` file to use Designer
- [ ] Review `ARCHITECTURE.md` to understand structure

---

**This manifest helps you navigate the complete project structure and understand each file's role.**
