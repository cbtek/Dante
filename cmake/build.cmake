# This file contains compiler flags for different platforms
message("${PROJECT_ROOT}/bin/Dante/release")
IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_ROOT}/bin/Dante/debug")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_ROOT}/bin/Dante/debug")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT}/bin/Dante/debug")
ELSE()
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_ROOT}/bin/Dante/release")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_ROOT}/bin/Dante/release")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT}/bin/Dante/release")
ENDIF()