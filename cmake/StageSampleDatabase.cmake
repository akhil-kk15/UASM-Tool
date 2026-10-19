# StageSampleDatabase.cmake
#
# Populates the sample database (SRC) next to the executable (DST) on every
# build so clicking Build in CLion, Visual Studio, Qt Creator, or running
# cmake --build always provides a freshly populated database.
#
# Usage:
#   cmake -DSRC=<source.db> -DDST=<dest.db> [-DSEED_SQL=<seed.sql>] -P StageSampleDatabase.cmake

if(NOT DEFINED SRC OR NOT DEFINED DST)
    message(FATAL_ERROR "SRC and DST must both be defined")
endif()

get_filename_component(_dst_dir "${DST}" DIRECTORY)
if(_dst_dir AND NOT EXISTS "${_dst_dir}")
    file(MAKE_DIRECTORY "${_dst_dir}")
endif()

if(EXISTS "${SRC}")
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E copy "${SRC}" "${DST}"
        RESULT_VARIABLE _copy_res
    )
    if(_copy_res EQUAL 0)
        message(STATUS "Populated sample database at: ${DST}")
    endif()
elseif(DEFINED SEED_SQL AND EXISTS "${SEED_SQL}")
    find_program(_sqlite3_exe NAMES sqlite3 sqlite3.exe)
    if(_sqlite3_exe)
        execute_process(
            COMMAND "${_sqlite3_exe}" "${DST}"
            INPUT_FILE "${SEED_SQL}"
            RESULT_VARIABLE _sqlite_res
        )
        if(_sqlite_res EQUAL 0)
            message(STATUS "Populated database from SQL seed at: ${DST}")
        endif()
    endif()
else()
    message(STATUS "Sample database not found at ${SRC}; skipping staging")
endif()
