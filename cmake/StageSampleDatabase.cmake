# StageSampleDatabase.cmake
#
# Copies the sample database (SRC) next to the executable (DST) only when the
# destination does not exist yet. This keeps rebuilds from overwriting a
# database the application has already written data into.
#
# Usage:
#   cmake -DSRC=<source.db> -DDST=<dest.db> -P StageSampleDatabase.cmake

if(NOT DEFINED SRC OR NOT DEFINED DST)
    message(FATAL_ERROR "SRC and DST must both be defined")
endif()

if(NOT EXISTS "${SRC}")
    message(STATUS "Sample database not found at ${SRC}; skipping staging")
    return()
endif()

if(EXISTS "${DST}")
    message(STATUS "Database already present at ${DST}; leaving it untouched")
    return()
endif()

configure_file("${SRC}" "${DST}" COPYONLY)
message(STATUS "Staged sample database: ${DST}")
