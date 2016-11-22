#==============================================================================
# CMakeLists file for dante
#==============================================================================
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CURR_TARGET dante)
set(CURR_TARGET_SOURCES "${CURR_TARGET}_SOURCES")
set(CURR_TARGET_HEADERS"${CURR_TARGET}_HEADERS")
include_directories(inc)
include_directories(inc/platform/msbuild)
include_directories(inc/platform/cmake)

include(${CMAKE_MODULE_PATH}/flags.cmake)

if (${ENABLE_DANTE_UNIT_TESTS})
    add_subdirectory(tests)
endif()

#==============================================================================
# Include Source Files
#==============================================================================
file(GLOB CURR_TARGET_SOURCES "src/*.cpp"
                              "src/platform/msbuild/*.cpp"
                              "src/platform/cmake/*.cpp")

file(GLOB CURR_TARGET_HEADERS "inc/*.h*"
                              "inc/platform/msbuild/*.h*"
                              "inc/platform/cmake/*.h*")

#==============================================================================
# Add Library
#==============================================================================
add_library(${CURR_TARGET}
            ${LIBRARY_MODE}
            ${CURR_TARGET_SOURCES}
            ${CURR_TARGET_HEADERS})

add_dependencies(${CURR_TARGET}
                 common_utils)
