# Application Architecture & Design Document

## System Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    Qt GUI Layer (UI)                         │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              MainWindow (mainwindow.ui)              │   │
│  │  ┌──────────────┬──────────────┬─────────────────┐  │   │
│  │  │  Employees   │   Uniforms   │  Issue/Return   │  │   │
│  │  │     Tab      │     Tab      │      Tab        │  │   │
│  │  └──────────────┴──────────────┴─────────────────┘  │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                               │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              Dialog Windows                          │   │
│  │  ├─ Employee Dialog (employeededialog.ui)           │   │
│  │  ├─ Uniform Dialog (uniformdialog.ui)               │   │
│  │  └─ Issue Dialog (issuedialog.ui)                   │   │
│  └──────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
                              ▲
                              │
                         Calls signals/slots
                              │
┌─────────────────────────────────────────────────────────────┐
│               Business Logic Layer (Models)                  │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │  Employee    │  │   Uniform    │  │ UniformIssue │      │
│  │  (Data Class)│  │ (Data Class) │  │ (Data Class) │      │
│  └──────────────┘  └──────────────┘  └──────────────┘      │
│                                                               │
└─────────────────────────────────────────────────────────────┘
                              ▲
                              │
                      CRUD Operations
                              │
┌─────────────────────────────────────────────────────────────┐
│         Database Management Layer (DatabaseManager)          │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌────────────────────────────────────────────────────┐    │
│  │ DatabaseManager (Singleton Pattern)                │    │
│  │ - Initialize database                             │    │
│  │ - Employee CRUD operations                        │    │
│  │ - Uniform CRUD operations                         │    │
│  │ - Uniform Issue CRUD operations                   │    │
│  │ - Error handling                                  │    │
│  └────────────────────────────────────────────────────┘    │
│                                                               │
└─────────────────────────────────────────────────────────────┘
                              ▲
                              │
                      SQL Queries
                              │
┌─────────────────────────────────────────────────────────────┐
│            Data Persistence Layer (SQLite Database)          │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │  employees   │  │   uniforms   │  │ uniform_     │      │
│  │   table      │  │    table     │  │  issues      │      │
│  │              │  │              │  │   table      │      │
│  └──────────────┘  └──────────────┘  └──────────────┘      │
│                                                               │
│         uniformmanagement.db (SQLite File)                  │
└─────────────────────────────────────────────────────────────┘
```

## Class Diagram

```
┌─────────────────────────────────────┐
│          MainWindow                 │
│  (QMainWindow)                      │
├─────────────────────────────────────┤
│ - ui: Ui::MainWindow*               │
│ - m_db: DatabaseManager             │
├─────────────────────────────────────┤
│ + loadEmployeesTable()              │
│ + loadUniformsTable()               │
│ + loadIssuesTable()                 │
│ + on_addEmployeeBtn_clicked()       │
│ + on_editEmployeeBtn_clicked()      │
│ ... (more slots)                    │
└─────────────────────────────────────┘
           │ contains
           └──▶ DatabaseManager
           
┌─────────────────────────────────────┐
│      DatabaseManager                │
├─────────────────────────────────────┤
│ - m_database: QSqlDatabase          │
│ - m_lastError: QString              │
├─────────────────────────────────────┤
│ + initializeDatabase(): bool        │
│ + createTables(): bool              │
│ + addEmployee(emp): bool            │
│ + getEmployee(id): Employee         │
│ + getAllEmployees(): QList<E>       │
│ + updateEmployee(emp): bool         │
│ + deleteEmployee(id): bool          │
│ ... (similar for Uniform & Issue)   │
│ + getLastError(): QString           │
└─────────────────────────────────────┘

┌──────────────┐  ┌──────────────┐  ┌─────────────────┐
│  Employee    │  │   Uniform    │  │  UniformIssue   │
│ (Data Class) │  │ (Data Class) │  │  (Data Class)   │
├──────────────┤  ├──────────────┤  ├─────────────────┤
│ - m_id       │  │ - m_id       │  │ - m_id          │
│ - m_name     │  │ - m_type     │  │ - m_employeeId  │
│ - m_desg     │  │ - m_quantity │  │ - m_uniformId   │
│ - m_dept     │  │ - m_size     │  │ - m_quantity    │
│ - m_email    │  │ - m_color    │  │ - m_issueDate   │
│ - m_phone    │  │ - m_date     │  │ - m_returnDate  │
│ - m_size     │  │ - m_cost     │  │ - m_status      │
│ - m_joinDate │  │ - m_desc     │  │                 │
│ - m_active   │  │              │  │                 │
├──────────────┤  ├──────────────┤  ├─────────────────┤
│ + getId()    │  │ + getId()    │  │ + getId()       │
│ + getName()  │  │ + getType()  │  │ + getEmplId()   │
│ + setName()  │  │ + setType()  │  │ + setEmplId()   │
│ ... getters/ │  │ ... getters/ │  │ ... getters/    │
│     setters  │  │     setters  │  │     setters     │
└──────────────┘  └──────────────┘  └─────────────────┘
```

## Data Flow Diagram

### Adding a New Employee

```
User Interface Layer
    │
    ├─→ User clicks "Add Employee" button
    │
Application Layer
    ├─→ EmployeeDialog opens
    ├─→ User fills form and clicks OK
    ├─→ Dialog creates Employee object
    ├─→ MainWindow receives Employee data
    │
Business Logic Layer
    ├─→ Create Employee object with form data
    │
Database Layer
    ├─→ DatabaseManager::addEmployee(employee)
    ├─→ Execute SQL: INSERT INTO employees (...)
    │
Persistence Layer
    ├─→ SQLite stores data in employees table
    │
Application Layer
    ├─→ Refresh employees table display
    ├─→ Show success message to user
    │
User Interface Layer
    └─→ Updated table visible to user
```

### Issuing a Uniform

```
User Interface Layer
    │
    ├─→ User selects "Issue Uniform"
    │
Application Layer
    ├─→ IssueDialog opens
    ├─→ Load employee and uniform lists from DB
    ├─→ User selects employee, uniform, quantity
    ├─→ User clicks OK
    │
Business Logic Layer
    ├─→ Create UniformIssue object
    │
Database Layer
    ├─→ DatabaseManager::issueUniform(issue)
    ├─→ Execute SQL: INSERT INTO uniform_issues (...)
    │
Persistence Layer
    ├─→ SQLite stores transaction record
    │
Application Layer
    ├─→ Refresh issues table
    ├─→ Show confirmation message
    │
User Interface Layer
    └─→ Issue appears in Issue/Return tab table
```

## File Organization

### Header Files (.h)
- `employee.h` - Employee data model declaration
- `uniform.h` - Uniform inventory model
- `uniformissue.h` - Uniform transaction model
- `databasemanager.h` - Database operations interface
- `mainwindow.h` - Main window class
- `employeedialog.h` - Employee dialog class
- `uniformdialog.h` - Uniform dialog class
- `issuedialog.h` - Issue dialog class

### Implementation Files (.cpp)
- `employee.cpp` - Employee methods
- `uniform.cpp` - Uniform methods
- `uniformissue.cpp` - UniformIssue methods
- `databasemanager.cpp` - Database implementation
- `mainwindow.cpp` - Main window slots and logic
- `employeedialog.cpp` - Employee form logic
- `uniformdialog.cpp` - Uniform form logic
- `issuedialog.cpp` - Issue form logic
- `main.cpp` - Application entry point

### UI Files (.ui)
- `mainwindow.ui` - Main window with 3 tabs
- `employeededialog.ui` - Employee form dialog
- `uniformdialog.ui` - Uniform form dialog
- `issuedialog.ui` - Issue form dialog

### Configuration
- `CMakeLists.txt` - Build configuration

## Database Schema Relationships

```
┌──────────────────────────────┐
│      employees               │
├──────────────────────────────┤
│ PK: id (INTEGER)             │
│     name (TEXT)              │
│     designation (TEXT)       │
│     department (TEXT)        │
│     email (TEXT)             │
│     phone (TEXT)             │
│     uniform_size (TEXT)      │
│     joining_date (DATE)      │
│     active (BOOLEAN)         │
└──────────────────────────────┘
           ▲
           │ (1:N relationship)
           │
┌──────────────────────────────┐
│    uniform_issues            │
├──────────────────────────────┤
│ PK: id (INTEGER)             │
│ FK: employee_id ────────────→│ references id
│ FK: uniform_id               │
│     quantity (INTEGER)       │
│     issue_date (DATE)        │
│     return_date (DATE)       │
│     status (TEXT)            │
└──────────────────────────────┘
           ▲
           │ (N:1 relationship)
           │
┌──────────────────────────────┐
│      uniforms                │
├──────────────────────────────┤
│ PK: id (INTEGER)             │
│     type (TEXT)              │
│     quantity (INTEGER)       │
│     size (TEXT)              │
│     purchase_date (DATE)     │
│     cost (REAL)              │
│     description (TEXT)       │
│     color (TEXT)             │
└──────────────────────────────┘
```

## Design Patterns Used

### 1. **Model-View Pattern**
- Models (Employee, Uniform, UniformIssue) store data
- Views (Tables, Dialogs) display data
- Controllers (Slots) handle interactions

### 2. **Manager Pattern**
- DatabaseManager centralizes all database operations
- Provides single interface for all DB interactions
- Encapsulates SQL complexity

### 3. **DAO (Data Access Object) Pattern**
- DatabaseManager acts as DAO
- Separates business logic from data access
- Enables future database switching

### 4. **Observer Pattern (Qt Signals/Slots)**
- Button clicks trigger slots
- Dialogs emit signals when data changes
- UI updates reactively

## User Interface Design

### Main Window Layout (3 Tabs)

**Tab 1: Employees**
```
┌─────────────────────────────────────────┐
│ [Add] [Edit] [Delete] [Refresh]        │
├─────────────────────────────────────────┤
│ ID │ Name │ Designation │ Dept │ ... │
├─────────────────────────────────────────┤
│ 1  │ John │ Manager     │ IT   │ ... │
│ 2  │ Jane │ Developer   │ IT   │ ... │
└─────────────────────────────────────────┘
```

**Tab 2: Uniforms**
```
┌─────────────────────────────────────────┐
│ [Add] [Edit] [Delete] [Refresh]        │
├─────────────────────────────────────────┤
│ ID │ Type │ Qty │ Size │ Color │ ... │
├─────────────────────────────────────────┤
│ 1  │ T-Shirt │ 50 │ M │ Blue │ ...  │
│ 2  │ Pants   │ 30 │ L │ Black│ ...  │
└─────────────────────────────────────────┘
```

**Tab 3: Issue/Return**
```
┌──────────────────────────────────────────┐
│ [Issue] [Return] [Refresh]              │
├──────────────────────────────────────────┤
│ ID │ Employee │ Uniform │ Qty │ Status │
├──────────────────────────────────────────┤
│ 1  │ John     │ T-Shirt │ 1   │ Issued │
│ 2  │ Jane     │ Pants   │ 1   │ Issued │
└──────────────────────────────────────────┘
```

## Error Handling Strategy

1. **Database Errors**: Caught in DatabaseManager, reported to UI
2. **User Input Validation**: Dialogs validate before submission
3. **User Feedback**: MessageBox dialogs show status/errors
4. **Error Logging**: All errors stored in m_lastError for debugging

## Future Enhancement Points

1. **Authentication**: Add user login and roles
2. **Reports**: Generate PDF/Excel reports
3. **Search**: Add search and filter functionality
4. **Maintenance**: Track uniform maintenance history
5. **Notifications**: Email alerts for returns due
6. **Audit Trail**: Log all changes with user info
7. **Multi-user**: Support concurrent database access
8. **Barcode**: QR/Barcode scanning support
