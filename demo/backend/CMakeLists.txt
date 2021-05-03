cmake_minimum_required(VERSION 3.10)

# set the project name
project(Marbellous)

include_directories(include/)

file(GLOB SOURCES src/*.cpp)

# add the executable
add_executable(Marbellous ${SOURCES})

# emcc flags
if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set_target_properties(Marbellous PROPERTIES LINK_FLAGS "-s EXTRA_EXPORTED_RUNTIME_METHODS='[ccall]' -O3 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1 -s FULL_ES3=1 -s ASSERTIONS=1")
endif()