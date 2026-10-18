# DeployQt.cmake
#
# Runs windeployqt on EXE to place the Qt DLLs, plugins (platform + sqldrivers)
# and the compiler runtime next to the executable - but only when the runtime
# is not deployed there yet, so repeated builds stay fast.
#
# Usage:
#   cmake -DEXE=<exe path> -DWINDEPLOYQT=<windeployqt path> -DQT_CONFIG_ARG=<--debug|--release> -P DeployQt.cmake

if(NOT DEFINED EXE OR NOT DEFINED WINDEPLOYQT)
    message(FATAL_ERROR "EXE and WINDEPLOYQT must both be defined")
endif()

get_filename_component(_exe_dir "${EXE}" DIRECTORY)

# Already deployed? (either Qt 6 or Qt 5 runtime next to the executable)
if(EXISTS "${_exe_dir}/Qt6Core.dll" OR EXISTS "${_exe_dir}/Qt5Core.dll")
    return()
endif()

if(NOT DEFINED QT_CONFIG_ARG OR QT_CONFIG_ARG STREQUAL "")
    set(QT_CONFIG_ARG --release)
endif()

execute_process(
    COMMAND "${WINDEPLOYQT}" ${QT_CONFIG_ARG}
            --no-translations
            --no-system-d3d-compiler
            --no-opengl-sw
            "${EXE}"
    RESULT_VARIABLE _result)

if(NOT _result EQUAL 0)
    message(WARNING "windeployqt failed (exit code ${_result}); the executable may not "
                    "start outside Visual Studio. See README.md.")
else()
    message(STATUS "Qt runtime deployed next to ${EXE}")
endif()
