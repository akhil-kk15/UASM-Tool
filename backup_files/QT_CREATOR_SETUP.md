# Qt Creator Setup Guide for Uniform Management System

## Quick Start Guide

This document provides step-by-step instructions to open and work with the Uniform Management System in Qt Creator.

## Prerequisites

Before starting, ensure you have:
- **Qt Creator** 4.13 or later installed
- **Qt 5.15** or **Qt 6.x** with:
  - Qt Widgets module
  - Qt SQL module
- **CMake** 3.16 or higher
- A C++17 capable compiler (GCC, Clang, or MSVC)

## Step 1: Opening the Project in Qt Creator

### Method 1: Using File Menu
1. Open Qt Creator
2. Click **File** → **Open File or Project**
3. Navigate to `/home/akhilkk/Projects/AIDS/untitled1`
4. Select **CMakeLists.txt**
5. Click **Open**

### Method 2: Using Command Line
```bash
cd /home/akhilkk/Projects/AIDS/untitled1
qtcreator CMakeLists.txt &
```

## Step 2: Configuring the Project

After opening, Qt Creator will prompt you to configure the project:

1. **Select Kits**: Choose your desired build kit (e.g., "Desktop Qt 6.x GCC 64bit")
2. **Click Configure Project**
3. Qt will run CMake to generate build files
4. Wait for the configuration to complete (you'll see green checkmarks)

## Step 3: Building the Project

### Option A: Using Qt Creator Interface
1. Press **Ctrl + B** to build
2. Or go to **Build** → **Build All**
3. Wait for compilation to complete

### Option B: Using Terminal
```bash
cd /home/akhilkk/Projects/AIDS/untitled1/build
make   # or ninja if using Ninja generator
```

## Step 4: Running the Application

1. Press **Ctrl + R** to run, or
2. Click the green **▶ Run** button in the bottom-left
3. The application window will open

## Editing UI Files in Qt Designer

Qt Designer is integrated into Qt Creator and allows visual editing of `.ui` files. This is one of the main advantages of using Qt Creator.

### Opening UI Files

1. **In Project Explorer** (left panel), locate the `.ui` files:
   - `mainwindow.ui` - Main application window
   - `employeededialog.ui` - Employee form dialog
   - `uniformdialog.ui` - Uniform form dialog
   - `issuedialog.ui` - Uniform issue dialog

2. **Double-click any `.ui` file** to open it in Qt Designer

### Using Qt Designer

#### Main Window (mainwindow.ui)
- Three tabs for Employees, Uniforms, and Issue/Return
- Each tab contains:
  - Action buttons (Add, Edit, Delete, Refresh)
  - A QTableWidget for displaying data
- **To customize:**
  - Drag and drop widgets from the left panel
  - Right-click widgets to access properties
  - Modify layout spacing and alignment
  - Change button labels, colors, and icons

#### Dialog Windows
- Employee, Uniform, and Issue dialogs use form layouts
- Fields are grouped logically with labels
- **To customize:**
  - Add new fields by dragging QLineEdit, QSpinBox, etc.
  - Modify form layout properties
  - Adjust validators and input ranges

### Making Changes in Designer

1. Drag widgets from the **Widget Box** (left side)
2. Arrange them in your layout
3. Right-click widgets to access **Edit Buddies** or change properties
4. Modify **Object Names** to match code expectations
5. Save with **Ctrl + S**

**Important:** When you save a `.ui` file, Qt Creator automatically regenerates the corresponding C++ header file. This is compiled automatically.

## Project Structure in Qt Creator

When you open the project, you'll see:

```
UniformManagementSystem
├── CMakeLists.txt
├── Sources
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── employeedialog.cpp
│   ├── uniformdialog.cpp
│   ├── issuedialog.cpp
│   ├── databasemanager.cpp
│   ├── employee.cpp
│   ├── uniform.cpp
│   └── uniformissue.cpp
├── Headers
│   ├── mainwindow.h
│   ├── employeedialog.h
│   ├── uniformdialog.h
│   ├── issuedialog.h
│   ├── databasemanager.h
│   ├── employee.h
│   ├── uniform.h
│   └── uniformissue.h
├── Forms
│   ├── mainwindow.ui
│   ├── employeededialog.ui
│   ├── uniformdialog.ui
│   └── issuedialog.ui
└── Resources
    └── CMakeLists.txt
```

## Common Tasks

### Adding a New Feature by Modifying UI

1. Open the relevant `.ui` file in Designer
2. Add new widgets (buttons, fields, etc.)
3. Set the **Object Name** of the widget (used in code)
4. Set the **Tool Tip** and **Status Tip** for user help
5. Save the file
6. In the corresponding `.cpp` file, connect new signals/slots:
   ```cpp
   connect(ui->newButton, &QPushButton::clicked, this, &MainWindow::on_newButton_clicked);
   ```

### Debugging

Use Qt Creator's built-in debugger:

1. Set a breakpoint by clicking on the line number
2. Press **F5** or **Debug** → **Start Debugging**
3. Application will run and stop at breakpoints
4. Inspect variables in the **Locals** panel (right side)

### Viewing Application Output

1. Build and run the application
2. Output appears in the **Application Output** tab (bottom)
3. Qt automatically shows:
   - Debug messages from `qDebug()`
   - SQL errors
   - Compilation warnings

## Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| Ctrl + B | Build project |
| Ctrl + R | Run project |
| F5 | Debug project |
| Ctrl + F | Find in code |
| Ctrl + / | Toggle comment |
| Ctrl + Shift + D | Delete line |
| F2 | Open help |

## Troubleshooting

### "Kit not configured" Error
- Click on **Projects** (left sidebar)
- Select your kit
- Choose the correct CMake version and compiler
- Click **Apply**

### Build Fails with "Qt not found"
- Ensure Qt is installed in the expected location
- In **Tools** → **Options** → **Kits**, verify Qt installation path
- Or set CMAKE_PREFIX_PATH manually

### UI Changes Not Appearing
- Clean the build: **Build** → **Clean All**
- Rebuild: **Ctrl + B**
- This regenerates the UI header files

### Database File Issues
- First run creates `uniformmanagement.db` in the build directory
- To reset: delete the database file and restart
- Check permissions if you can't write to the directory

## Tips & Best Practices

1. **Use Designer for UI:** It's faster and error-free than manual code
2. **Test Incrementally:** Build and run after each significant change
3. **Study Generated Code:** Qt generates code from `.ui` files - review it in `build/` folder
4. **Use Qt Help:** Press F1 to access Qt documentation within Creator
5. **Version Control:** You can safely commit `.ui` files and `.h` files, but ignore `build/` and generated files

## Resources

- **Qt Documentation:** Available in Help (F1)
- **Official Qt Creator Manual:** https://doc.qt.io/qtcreator/
- **Qt Widget Documentation:** https://doc.qt.io/qt-5/qwidget.html
- **CMake Reference:** https://cmake.org/documentation/

## Next Steps

Once the project is built and running:

1. **Add test data:** Use the Add buttons in each tab
2. **Modify the UI:** Open `.ui` files in Designer
3. **Extend functionality:** Edit `.h` and `.cpp` files
4. **Deploy:** Build Release version (Debug → Release in kit selector)
