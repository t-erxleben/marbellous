cmake_minimum_required(VERSION 3.10)

# set the project name
project(Marbellous)

include_directories(include/ include/earcut/include/mapbox/)

file(GLOB SOURCES src/*.cpp)

# add the executable
add_executable(Marbellous ${SOURCES})
add_executable(Marbellous-debug EXCLUDE_FROM_ALL ${SOURCES})
set_target_properties(Marbellous Marbellous-debug PROPERTIES CXX_STANDARD 17)

# emcc flags
if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	set_target_properties(Marbellous PROPERTIES LINK_FLAGS "-s EXTRA_EXPORTED_RUNTIME_METHODS='[ccall,cwrap]' -O3 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=[\"_dropColor\",\"_main\"] -s USE_WEBGL2=1 -s FULL_ES3=1 -s ASSERTIONS=1")
	set_target_properties(Marbellous-debug PROPERTIES LINK_FLAGS "-fsanitize=undefined,address -g4 -s ASSERTIONS=1 -s TOTAL_MEMORY=278790144  -s EXTRA_EXPORTED_RUNTIME_METHODS='[ccall,cwrap]' -O3 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=[\"_dropColor\",\"_main\"] -s USE_WEBGL2=1 -s FULL_ES3=1 -s ASSERTIONS=1 -s LLD_REPORT_UNDEFINED")
	set_target_properties(Marbellous-debug PROPERTIES COMPILE_FLAGS "-fsanitize=undefined,address -s ASSERTIONS=1 -g4")
endif()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Marbellous.js ${CMAKE_CURRENT_BINARY_DIR}/Marbellous.wasm DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/page/)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Marbellous-debug.js DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/page/
	RENAME Marbellous.js COMPONENT Marbellous-debug EXCLUDE_FROM_ALL)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Marbellous-debug.wasm ${CMAKE_CURRENT_BINARY_DIR}/Marbellous-debug.wasm.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/page/
	COMPONENT Marbellous-debug EXCLUDE_FROM_ALL)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/../frontend/dist/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/page/ COMPONENT Marbellous-debug)

