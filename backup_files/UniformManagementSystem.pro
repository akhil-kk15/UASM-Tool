QT += core gui sql

CONFIG += c++17

TARGET = UniformManagementSystem
TEMPLATE = app

# Source files
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    employee.cpp \
    uniform.cpp \
    uniformissue.cpp \
    databasemanager.cpp \
    employeedialog.cpp \
    uniformdialog.cpp \
    issuedialog.cpp

# Header files
HEADERS += \
    mainwindow.h \
    employee.h \
    uniform.h \
    uniformissue.h \
    databasemanager.h \
    employeedialog.h \
    uniformdialog.h \
    issuedialog.h

# UI files
FORMS += \
    mainwindow.ui \
    employeededialog.ui \
    uniformdialog.ui \
    issuedialog.ui

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# UI Headers path
UI_DIR = ./ui_generated
MOC_DIR = ./moc_generated
OBJECTS_DIR = ./obj
RCC_DIR = ./rcc_generated
