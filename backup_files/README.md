# Uniform Management System - Qt Application

## Project Overview

The **Uniform Management System** is a Qt/C++ desktop application designed to manage existing employee uniforms and accessories in a workplace. It provides functionality to track employee information, manage uniform inventory (without pricing), and record issue/return transactions.

## Features

### 1. **Employee Management**
- Add new employees with details (name, designation, department, email, phone, uniform size)
- Edit employee information
- Delete employee records
- View all active employees in a table format
- Track employee joining dates

### 2. **Uniform Management**
- Add uniform types to inventory (e.g., T-shirt, Pants, Safety Vest)
- Track uniform quantities and sizes
- Store purchase dates
- Add descriptions and color information
- Edit and delete uniform records

### 3. **Issue/Return Tracking**
- Issue uniforms to employees
- Record return of uniforms
- Maintain transaction history with dates
- Track uniform status (Issued, Returned, Pending)
- View issue history for each employee

## Application Architecture

### Data Models

#### **Employee.h/cpp**
- Stores employee information
- Properties: ID, Name, Designation, Department, Email, Phone, Uniform Size, Joining Date, Active Status
- Getters and setters for all properties

#### **Uniform.h/cpp**
- Stores uniform inventory information
- Properties: ID, Type, Quantity, Size, Color, Purchase Date, Cost, Description
- CRUD operations support

#### **UniformIssue.h/cpp**
- Tracks uniform transactions
- Properties: ID, Employee ID, Uniform ID, Quantity, Issue Date, Return Date, Status
- Links employees with uniforms

### Database Layer

#### **DatabaseManager.h/cpp**
- Manages SQLite database operations
- Creates and maintains three main tables:
  - `employees`: Employee records
  - `uniforms`: Uniform inventory
  - `uniform_issues`: Transaction history
- Methods for CRUD operations on all entities
- Error handling with detailed messages

### User Interface

#### **MainWindow**
- Three-tab interface:
  1. **Employees Tab**: Manage employee records
  2. **Uniforms Tab**: Manage uniform inventory
  3. **Issue/Return Tab**: Track uniform transactions
- Each tab has action buttons and a data table
- Status bar for user feedback

#### **Dialog Classes**
- **EmployeeDialog** (employeedialog.h/cpp): For adding/editing employees
- **UniformDialog** (uniformdialog.h/cpp): For adding/editing uniforms
- **IssueDialog** (issuedialog.h/cpp): For issuing uniforms

## Database Schema

### employees table
```
id INTEGER PRIMARY KEY AUTOINCREMENT
name TEXT NOT NULL
designation TEXT
department TEXT
email TEXT
phone TEXT
uniform_size TEXT
joining_date DATE
active BOOLEAN DEFAULT 1
```

### uniforms table
```
id INTEGER PRIMARY KEY AUTOINCREMENT
type TEXT NOT NULL
quantity INTEGER
size TEXT
purchase_date DATE
description TEXT
color TEXT
```

### uniform_issues table
```
id INTEGER PRIMARY KEY AUTOINCREMENT
employee_id INTEGER NOT NULL (FOREIGN KEY)
uniform_id INTEGER NOT NULL (FOREIGN KEY)
quantity INTEGER
issue_date DATE
return_date DATE
status TEXT DEFAULT 'Issued'
```

## Project Structure

```
untitled1/
├── CMakeLists.txt                    # Build configuration for Qt
├── main.cpp                          # Application entry point
├── mainwindow.h / mainwindow.cpp     # Main window implementation
├── mainwindow.ui                     # Main window UI design (Qt Designer editable)
├── employee.h / employee.cpp         # Employee data model
├── uniform.h / uniform.cpp           # Uniform data model
├── uniformissue.h / uniformissue.cpp # Uniform issue data model
├── databasemanager.h / databasemanager.cpp  # Database operations
├── employeedialog.h / employeedialog.cpp    # Employee dialog
├── employeededialog.ui               # Employee dialog UI design
├── uniformdialog.h / uniformdialog.cpp      # Uniform dialog
├── uniformdialog.ui                  # Uniform dialog UI design
├── issuedialog.h / issuedialog.cpp          # Issue dialog
├── issuedialog.ui                    # Issue dialog UI design
└── uniformmanagement.db              # SQLite database (created at runtime)
```

## Opening in Qt Creator

### Prerequisites
- Qt 5.15 or Qt 6.x installed
- Qt Creator installed
- CMake 3.16 or higher
- C++17 capable compiler

### Steps to Open

1. **Open Qt Creator**

2. **File → Open File or Project**

3. **Navigate to the project directory** and select `CMakeLists.txt`

4. **Qt Creator will automatically configure the project**

5. **Edit UI Files in Qt Designer:**
   - Double-click on any `.ui` file (e.g., `mainwindow.ui`)
   - Qt Designer will open, allowing visual editing
   - Changes are automatically compiled
   - Main UI files to design:
     - `mainwindow.ui` - Main application window with tabs
     - `employeededialog.ui` - Employee form dialog
     - `uniformdialog.ui` - Uniform form dialog
     - `issuedialog.ui` - Uniform issue dialog

6. **Build the project:**
   - Select Build Kit (Desktop)
   - Build → Build All
   - Or press Ctrl+B

## Building and Running

### Using Qt Creator
1. Select your kit in the bottom-left
2. Press Ctrl+B to build
3. Press Ctrl+R to run

### Using Command Line

```bash
cd /home/akhilkk/Projects/AIDS/untitled1

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake -DCMAKE_PREFIX_PATH=<Qt installation path> ..

# Build
make

# Run
./UniformManagementSystem
```

**Example for Qt 6:**
```bash
cmake -DCMAKE_PREFIX_PATH=/opt/Qt/6.x.x/gcc_64 ..
```

## Features Implemented

### Core Functionality
- ✅ Employee CRUD operations
- ✅ Uniform inventory management
- ✅ Issue/Return tracking
- ✅ SQLite database persistence
- ✅ Table view for all entities
- ✅ Dialog-based forms for data entry
- ✅ Error handling and user feedback

### UI/UX
- ✅ Tab-based interface for organization
- ✅ Qt Designer compatible UI files
- ✅ Consistent button layout
- ✅ Data validation in dialogs
- ✅ Confirmation dialogs for destructive operations

## Usage Guide

### Adding an Employee
1. Go to the "Employees" tab
2. Click "Add Employee"
3. Fill in the employee details
4. Select uniform size from dropdown
5. Click OK

### Managing Uniforms
1. Go to the "Uniforms" tab
2. Click "Add Uniform"
3. Enter uniform details (type, quantity, size, color, cost)
4. Click OK

### Issuing Uniforms
1. Go to the "Issue/Return" tab
2. Click "Issue Uniform"
3. Select employee and uniform type
4. Enter quantity
5. Confirm issue date
6. Click OK

### Returning Uniforms
1. Go to the "Issue/Return" tab
2. Select the issued uniform from the table
3. Click "Return Uniform"
4. System records return date automatically

## Technologies Used

- **Language:** C++17
- **Framework:** Qt (Qt 5.15 or Qt 6.x)
- **Database:** SQLite
- **Build System:** CMake
- **UI Framework:** Qt Widgets

## Future Enhancements

- Export data to Excel/PDF
- Uniform maintenance tracking
- Employee uniform history reports
- Cost analysis and budgeting
- Search and filter functionality
- User authentication
- Barcode/QR code support
- Multi-user support with roles

## Troubleshooting

### Database Not Found
- Delete the build folder and rebuild
- Ensure write permissions in project directory

### UI Not Loading
- Check that all `.ui` files are present
- Rebuild the project to regenerate headers

### Missing Dependencies
- Ensure Qt with Widgets and SQL modules are installed
- Update CMAKE_PREFIX_PATH to correct Qt installation

## Notes

- The database file `uniformmanagement.db` is created automatically on first run
- All UI files are fully editable in Qt Designer
- The application uses responsive layouts that adapt to window resizing
- All operations include proper error handling and user notifications
