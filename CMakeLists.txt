cmake_minimum_required(VERSION 3.16)

project(UASM-Tool VERSION 1.0.0 LANGUAGES CXX)

# Export compile flags for VS Code / clangd IntelliSense (ignored by the
# Visual Studio generator on Windows).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8 /W3)
endif()

# ---------------------------------------------------------------------------
# Locate Qt automatically so Visual Studio / VS Code can configure without
# hand-written Qt paths:
#   1. -DCMAKE_PREFIX_PATH=... passed on the command line always wins
#   2. the QTDIR environment variable
#   3. the standard installation folders (Qt online installer layout):
#        Windows: C:/Qt/<version>/<kit>, D:/Qt/<version>/<kit>, %USERPROFILE%/Qt
#        Linux:   ~/Qt/<version>/<kit>, /opt/Qt/<version>/<kit>
#      When several kits are installed, the newest version is picked. Only kits
#      matching the active compiler are considered on Windows: MSVC cannot link
#      against a MinGW Qt and vice versa.
# ---------------------------------------------------------------------------
if(NOT CMAKE_PREFIX_PATH AND DEFINED ENV{QTDIR})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

if(NOT CMAKE_PREFIX_PATH)
    set(_qt_roots "")
    if(WIN32)
        list(APPEND _qt_roots "C:/Qt" "D:/Qt" "$ENV{USERPROFILE}/Qt")
    else()
        list(APPEND _qt_roots "$ENV{HOME}/Qt" "/opt/Qt")
    endif()

    set(_qt_candidates "")
    foreach(_qt_root IN LISTS _qt_roots)
        if(IS_DIRECTORY "${_qt_root}")
            file(GLOB _qt_found
                "${_qt_root}/*/msvc*_64"
                "${_qt_root}/*/mingw*"
                "${_qt_root}/*/gcc_64"
                "${_qt_root}/*/clang_64"
                "${_qt_root}/*/macos")
            list(APPEND _qt_candidates ${_qt_found})
        endif()
    endforeach()

    # Only consider kits that match the active compiler: MSVC cannot link
    # against a MinGW Qt and vice versa.
    set(_qt_matching "")
    foreach(_qt_candidate IN LISTS _qt_candidates)
        if(WIN32)
            if(MSVC AND NOT _qt_candidate MATCHES "msvc")
                continue()
            elseif(NOT MSVC AND NOT _qt_candidate MATCHES "mingw")
                continue()
            endif()
        endif()
        list(APPEND _qt_matching "${_qt_candidate}")
    endforeach()

    set(_qt_best "")
    set(_qt_best_score -1)
    foreach(_qt_candidate IN LISTS _qt_matching)
        get_filename_component(_qt_version_dir "${_qt_candidate}" DIRECTORY)
        get_filename_component(_qt_version "${_qt_version_dir}" NAME)
        if(NOT _qt_version MATCHES "^[0-9]+\\.[0-9]+")
            continue()
        endif()

        string(REGEX MATCHALL "[0-9]+" _qt_ver_parts "${_qt_version}")
        list(GET _qt_ver_parts 0 _qt_major)
        list(GET _qt_ver_parts 1 _qt_minor)
        set(_qt_patch 0)
        list(LENGTH _qt_ver_parts _qt_ver_count)
        if(_qt_ver_count GREATER 2)
            list(GET _qt_ver_parts 2 _qt_patch)
        endif()
        math(EXPR _qt_score "${_qt_major} * 100000 + ${_qt_minor} * 1000 + ${_qt_patch}")

        if(_qt_score GREATER _qt_best_score)
            set(_qt_best_score ${_qt_score})
            set(_qt_best "${_qt_candidate}")
        endif()
    endforeach()

    if(_qt_best)
        list(APPEND CMAKE_PREFIX_PATH "${_qt_best}")
        message(STATUS "Qt auto-detected: ${_qt_best}")
    else()
        message(STATUS "Qt was not auto-detected. Pass -DCMAKE_PREFIX_PATH=<Qt kit dir> "
                       "or set the QTDIR environment variable.")
        if(WIN32 AND MSVC)
            message(STATUS "Note: MSVC requires the MSVC build of Qt; a MinGW-only Qt "
                           "installation cannot be used. See README.md.")
        endif()
    endif()
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
    # Stage the sample database next to the executable the first time only, so
    # rebuilding never overwrites a database the application has written data
    # into. Delete the copy next to the executable to re-stage it.
    add_custom_command(TARGET UASM-Tool POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -DSRC=${CMAKE_CURRENT_SOURCE_DIR}/uniformmanagement.db
            -DDST=$<TARGET_FILE_DIR:UASM-Tool>/uniformmanagement.db
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/StageSampleDatabase.cmake
        COMMENT "Staging sample database (only if missing)"
        VERBATIM
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

# ---------------------------------------------------------------------------
# Visual Studio convenience: run and debug straight from the IDE.
#   * Qt DLLs are added to the debugger PATH, so F5 works without windeployqt
#   * the working directory is the executable folder, next to which the sample
#     database is staged
# ---------------------------------------------------------------------------
if(WIN32 AND MSVC)
    get_target_property(_qt_qmake_location Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    if(_qt_qmake_location)
        get_filename_component(_qt_bin_dir "${_qt_qmake_location}" DIRECTORY)
        file(TO_NATIVE_PATH "${_qt_bin_dir}" _qt_bin_dir_native)
        set_target_properties(UASM-Tool PROPERTIES
            VS_DEBUGGER_ENVIRONMENT "PATH=${_qt_bin_dir_native};$ENV{PATH}"
            VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:UASM-Tool>")
        set_target_properties(test_rotation_uniform PROPERTIES
            VS_DEBUGGER_ENVIRONMENT "PATH=${_qt_bin_dir_native};$ENV{PATH}"
            VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:test_rotation_uniform>")
    endif()
endif()

# ---------------------------------------------------------------------------
# Windows: deploy the Qt runtime (DLLs, platform and SQL driver plugins, MinGW
# runtime) next to the executable the first time it is missing, so the built
# .exe starts from Explorer, a terminal or Visual Studio with no manual
# windeployqt command. Disable with -DUASM_DEPLOY_QT=OFF.
# ---------------------------------------------------------------------------
option(UASM_DEPLOY_QT "Deploy the Qt runtime next to the Windows executables after building" ON)

if(WIN32 AND UASM_DEPLOY_QT)
    get_target_property(_qt_qmake_for_deploy Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    if(_qt_qmake_for_deploy)
        get_filename_component(_qt_bin_for_deploy "${_qt_qmake_for_deploy}" DIRECTORY)
        set(_windeployqt "${_qt_bin_for_deploy}/windeployqt.exe")
        if(EXISTS "${_windeployqt}")
            if(MSVC)
                set(_windeployqt_config_arg "$<$<CONFIG:Debug>:--debug>$<$<NOT:$<CONFIG:Debug>>:--release>")
            else()
                # Official MinGW Qt kits ship release DLLs only.
                set(_windeployqt_config_arg "--release")
            endif()
            foreach(_deploy_target UASM-Tool test_rotation_uniform)
                add_custom_command(TARGET ${_deploy_target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND}
                        -DEXE=$<TARGET_FILE:${_deploy_target}>
                        -DWINDEPLOYQT=${_windeployqt}
                        -DQT_CONFIG_ARG=${_windeployqt_config_arg}
                        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DeployQt.cmake
                    COMMENT "Ensuring the Qt runtime is deployed next to ${_deploy_target}"
                    VERBATIM)
            endforeach()
        endif()
    endif()
endif()
