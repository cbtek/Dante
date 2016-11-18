#==============================================================================
# CMakeLists file for Dante Example
#==============================================================================
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CURR_TARGET cbtek_dante_example)

set(CURR_TARGET_SOURCES "${CURR_TARGET}_SOURCES")
set(CURR_TARGET_HEADERS "${CURR_TARGET}_HEADERS")

include_directories(src)
include_directories(inc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#==============================================================================
# Include Source Files
#==============================================================================
file(GLOB CURR_TARGET_SOURCES "src/*.cpp")
file(GLOB CURR_TARGET_HEADERS "inc/*.h*")


#==============================================================================
# Add Executable
#==============================================================================
add_executable(${CURR_TARGET}
               ${PLATFORM}
               ${CURR_TARGET_SOURCES}
               ${CURR_TARGET_HEADERS}
               )

#==============================================================================
# Add Dependencies
#==============================================================================
add_dependencies(${CURR_TARGET}
                 dante
                 common_utils
                 )

#==============================================================================
# Target Link Libraries
#==============================================================================
target_link_libraries(${CURR_TARGET}
                      dante
                      common_utils)

# Copy templates folder to build directory
# so that the exe can see them
#add_custom_command(TARGET ${CURR_TARGET} POST_BUILD
 #             COMMAND ${CMAKE_COMMAND} -E copy_directory
  #                 ${CMAKE_CURRENT_SOURCE_DIR}/contrib/windows_ssh $<TARGET_FILE_DIR:${CURR_TARGET}>/pscp)
