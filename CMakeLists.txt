cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Trigon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE
     SRC
     ${CMAKE_CURRENT_SOURCE_DIR}
     "src/*.cpp")
file(GLOB_RECURSE
     SCRIPTS
     ${CMAKE_CURRENT_SOURCE_DIR}
     "scripts/*.sh")
add_executable(trigon ${SRC})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/triangle")

find_package(Threads REQUIRED)
target_link_libraries(trigon ${CMAKE_TRHEAD_LIBS_INIT} triangle)
target_include_directories(trigon
                           PRIVATE "${PROJECT_SOURCE_DIR}/external/triangle/")
find_package(PNG)
if(PNG_FOUND)
  add_definitions(-DTRIGON_PNG)
  target_link_libraries(trigon ${PNG_LIBRARIES})
  target_include_directories(trigon PUBLIC ${PNG_INCLUDE_DIRS})
endif()
find_package(JPEG)
if(JPEG_FOUND)
  message(${JPEG_LIBRARIES})
  add_definitions(-DTRIGON_JPEG)
  target_link_libraries(trigon ${JPEG_LIBRARIES})
  target_include_directories(trigon PUBLIC ${JPEG_INCLUDE_DIR})
endif()

add_custom_command(TARGET trigon PRE_BUILD
                   COMMAND ${CMAKE_COMMAND}
                           -E
                           copy_directory
                           ${CMAKE_SOURCE_DIR}/scripts/
                           $<TARGET_FILE_DIR:trigon>)

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
  execute_process(COMMAND ${CMAKE_COMMAND}
                          -E
                          copy_if_different
                          ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
                          ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
endif()
