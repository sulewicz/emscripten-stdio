cmake_minimum_required(VERSION 3.0)
project(stdio-demo C)
set(CMAKE_C_STANDARD 99)

if(EMSCRIPTEN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s FORCE_FILESYSTEM -Wl,--wrap=fgets,--wrap=fflush")
endif()

# Adding our source files
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/") # Define PROJECT_INCLUDE to be the path to the include directory of the project

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine

if(EMSCRIPTEN)
  file(GLOB WEB_FILES "${CMAKE_SOURCE_DIR}/web/*")
  foreach(WEB_FILE ${WEB_FILES})
    get_filename_component(FILE_NAME ${WEB_FILE} NAME)
    configure_file(${WEB_FILE} ${CMAKE_BINARY_DIR}/${FILE_NAME} COPYONLY)
  endforeach()
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_BINARY_DIR}/stdio-demo.js ${CMAKE_BINARY_DIR}/site/stdio-demo.js)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_BINARY_DIR}/stdio-demo.wasm ${CMAKE_BINARY_DIR}/site/stdio-demo.wasm)
endif()
