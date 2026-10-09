# Qt Creator Project Setup Instructions

## Now That You Have the .pro File

The new `AIDS.pro` file allows Qt Creator to recognize this as a native Qt project.

## How to Open in Qt Creator (Updated)

### Method 1: Using the .pro File (Recommended)
1. Open Qt Creator
2. **File → Open File or Project**
3. Navigate to `/home/akhilkk/Projects/AIDS/untitled1`
4. Select **`AIDS.pro`** (the new file)
5. Click **Open**
6. Qt Creator will ask which kit to use - **select your desktop kit**
7. Click **Configure Project**

### Method 2: Using CMakeLists.txt (Alternative)
Still works! You can also use:
- Select **`CMakeLists.txt`** instead of `.pro`
- Qt Creator will use CMake instead of qmake

## What Changed

### New File Added
- **`AIDS.pro`** - Qt project file (qmake format)
  - Lists all source files
  - Lists all header files
  - Lists all UI files
  - Configures Qt modules (gui, sql)
  - Sets C++17 standard

### Two Build Systems Available

**Option A: Using .pro file (qmake)**
```
Easier for pure Qt development
Works with Qt Designer integration
Standard Qt project format
Recommended for UI-heavy projects
```

**Option B: Using CMakeLists.txt (cmake)**
```
Modern build system
Cross-platform support
Better for complex builds
Good for version control
```

**You can use either one - they both work!**

## Opening the Project Now

### Quick Steps
1. **File → Open File or Project**
2. **Select**: `UniformManagementSystem.pro` 
3. **Click Open**
4. **Configure with your kit**
5. **Build**: Ctrl + B
6. **Run**: Ctrl + R

## If Qt Creator Still Shows "Not Part of Any Project"

Try these steps:

### Step 1: Close the current view
- If you have files open, close them all

### Step 2: Open the .pro file correctly
- File → Open File or Project
- Navigate to the `.pro` file
- Ensure you're selecting the file, not just viewing it

### Step 3: Complete configuration
- Qt Creator will prompt for kit selection
- Choose your desktop kit (Desktop Qt x.x)
- Click "Configure Project"

### Step 4: Verify project loaded
- Check the "Projects" tab on the left
- You should see the project tree with files listed

## Troubleshooting

### "qmake not found"
- Qt Creator is looking for qmake executable
- Go to **Tools → Options → Kits**
- Verify your kit has qmake path set
- Or use CMakeLists.txt instead

### "Projects view shows nothing"
- Right-click project → **Set as Active**
- Or close and reopen the .pro file

### Want to switch build systems?
- Just open the other file:
  - `.pro` file → uses qmake
  - `CMakeLists.txt` → uses cmake

## Building the Project

### Using .pro file (qmake)
In Qt Creator:
- **Ctrl + B** to build
- **Ctrl + R** to run
- **F5** to debug

### Using terminal (if needed)
```bash
cd /home/akhilkk/Projects/AIDS/untitled1
qmake UniformManagementSystem.pro
make
./UniformManagementSystem
```

## Editing in Qt Designer

Once opened with .pro file:
1. **Projects tab** (left side)
2. Expand project tree
3. Double-click any `.ui` file
4. Qt Designer opens automatically
5. Edit visually
6. Save with Ctrl + S

## Key Files in This Project

| File | Purpose |
|------|---------|
| `UniformManagementSystem.pro` | Qt project file (NEW) |
| `CMakeLists.txt` | CMake build file (alternative) |
| `mainwindow.ui` | Main window design |
| `*dialog.ui` | Dialog designs |
| `*.h` | Header files |
| `*.cpp` | Implementation files |

## Recommended Workflow

1. **First Time Opening**
   - Use the `.pro` file method above
   - Qt Creator will set up everything

2. **Regular Development**
   - Use **Ctrl + B** to build
   - Use **Ctrl + R** to run
   - Use **Ctrl + /** to toggle comments
   - Double-click `.ui` files to edit in Designer

3. **Version Control**
   - Both `.pro` and `CMakeLists.txt` are tracked
   - Qt Creator config (`.pro.user`) is usually ignored

## If You Still Have Issues

Try this complete reset:
1. Close Qt Creator completely
2. Delete `.qtcreator` folder in the project
3. Open Qt Creator again
4. Open the `.pro` file
5. Configure with kit
6. Build and run

The `.pro` file I created includes all necessary files and configurations for Qt Creator to recognize and run your project properly.

---

**Try opening `UniformManagementSystem.pro` now - your project should be fully recognized!**
