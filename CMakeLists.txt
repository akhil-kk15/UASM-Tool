cmake_minimum_required(VERSION 3.16)

project(UASM-Tool VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8 /W3)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        ui/mainwindow.ui
        src/employee.h
        src/employee.cpp
        src/uniform.h
        src/uniform.cpp
        src/uniformissue.h
        src/uniformissue.cpp
        src/databasemanager.h
        src/databasemanager.cpp
        src/employeedialog.h
        src/employeedialog.cpp
        ui/employeededialog.ui
        src/uniformdialog.h
        src/uniformdialog.cpp
        ui/uniformdialog.ui
        src/issuedialog.h
        src/issuedialog.cpp
        ui/issuedialog.ui
        src/team.h
        src/team.cpp
        src/rotationscheduler.h
        src/rotationscheduler.cpp
        src/reassigndialog.h
        src/reassigndialog.cpp
        ui/reassigndialog.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(UASM-Tool
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(UASM-Tool SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(UASM-Tool
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(UASM-Tool PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.uasmtool)
endif()
set_target_properties(UASM-Tool PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS UASM-Tool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/uniformmanagement.db")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/uniformmanagement.db"
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    add_custom_command(TARGET UASM-Tool POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/uniformmanagement.db"
            "$<TARGET_FILE_DIR:UASM-Tool>/uniformmanagement.db"
        COMMENT "Staging sample uniformmanagement.db to executable directory"
    )
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UASM-Tool)
endif()

add_executable(test_rotation_uniform
    tests/test_rotation_uniform.cpp
    src/employee.h
    src/employee.cpp
    src/uniform.h
    src/uniform.cpp
    src/uniformissue.h
    src/uniformissue.cpp
    src/team.h
    src/team.cpp
    src/rotationscheduler.h
    src/rotationscheduler.cpp
    src/databasemanager.h
    src/databasemanager.cpp
)
target_link_libraries(test_rotation_uniform PRIVATE Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Core)

enable_testing()
add_test(NAME test_rotation_uniform COMMAND test_rotation_uniform)
