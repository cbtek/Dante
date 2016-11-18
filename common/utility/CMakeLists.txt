#==============================================================================
# CMakeLists file for common_utils
#==============================================================================
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CURR_TARGET common_utils)
set(CURR_TARGET_SOURCES "${CURR_TARGET}_SOURCES")
set(CURR_TARGET_HEADERS"${CURR_TARGET}_HEADERS")
include_directories(inc)
include_directories(src)


if (WIN32)
    if (MSVC)

    else()
        set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11")
        set(CMAKE_CXX_FLAGS_DEFAULT "-std=c++11")
        set(CMAKE_CXX_FLAGS_DEBUG  "-std=c++11")
    endif()

else()
    set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11")
    set(CMAKE_CXX_FLAGS_DEFAULT "-std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG  "-std=c++11")
endif()

if (${ENABLE_COMMON_UTILS_UNIT_TESTS})
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

