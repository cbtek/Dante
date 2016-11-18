#==============================================================================
# CMakeLists file for dante
#==============================================================================
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CURR_TARGET dante)
set(CURR_TARGET_SOURCES "${CURR_TARGET}_SOURCES")
set(CURR_TARGET_HEADERS"${CURR_TARGET}_HEADERS")
include_directories(inc)
include_directories(src)

if (WIN32)
    if (MINGW)
        add_definitions(-std=c++11)
    endif()
else()
    if (GCC)
        add_definitions(-std=c++11)
    endif()
endif()

if (${ENABLE_DANTE_UNIT_TESTS})
    add_subdirectory(tests)
endif()

#==============================================================================
# Include Source Files
#==============================================================================
file(GLOB CURR_TARGET_SOURCES "src/*.cpp")
file(GLOB CURR_TARGET_HEADERS "inc/*.h*")

#==============================================================================
# Add Library
#==============================================================================
add_library(${CURR_TARGET}
            ${LIBRARY_MODE}
            ${CURR_TARGET_SOURCES}
            ${CURR_TARGET_HEADERS})

