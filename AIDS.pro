QT += core gui sql

CONFIG += c++17

TARGET = UniformManagementSystem
TEMPLATE = app

# Include paths
INCLUDEPATH += $$PWD/src \
              $$PWD/ui_generated

# Source files
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/employee.cpp \
    src/uniform.cpp \
    src/uniformissue.cpp \
    src/databasemanager.cpp \
    src/employeedialog.cpp \
    src/uniformdialog.cpp \
    src/issuedialog.cpp

# Header files
HEADERS += \
    src/mainwindow.h \
    src/employee.h \
    src/uniform.h \
    src/uniformissue.h \
    src/databasemanager.h \
    src/employeedialog.h \
    src/uniformdialog.h \
    src/issuedialog.h

# UI files
FORMS += \
    ui/mainwindow.ui \
    ui/employeededialog.ui \
    ui/uniformdialog.ui \
    ui/issuedialog.ui

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# UI Headers path
UI_DIR = ./ui_generated
MOC_DIR = ./moc_generated
OBJECTS_DIR = ./obj
RCC_DIR = ./rcc_generated
