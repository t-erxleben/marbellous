cmake_minimum_required(VERSION 3.10)

# set the project name
project(Marbellous)

set(FRONTEND_DIR ${CMAKE_SOURCE_DIR}/../frontend/)
file(GLOB WEB_SOURCESS ${FRONTEND_DIR}/src/*)
file(GLOB SOURCES src/*.cpp)

configure_file(${FRONTEND_DIR}/package.json ${CMAKE_CURRENT_BINARY_DIR}/package.json)
configure_file(${FRONTEND_DIR}/webpack.config.js.in ${CMAKE_CURRENT_BINARY_DIR}/webpack.config.js)

include_directories(include/)

add_custom_command(OUTPUT npm.stamp COMMAND npm install WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/package.json)
add_custom_command(OUTPUT dist/index.js COMMAND npx webpack && touch dist/index.js WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} DEPENDS ${WEB_SOURCESS} npm.stamp ${CMAKE_CURRENT_BINARY_DIR}/webpack.config.js)

# add the executable
add_executable(Marbellous dist/index.js ${SOURCES})
add_executable(Marbellous-debug EXCLUDE_FROM_ALL dist/index.js ${SOURCES})
set_target_properties(Marbellous Marbellous-debug PROPERTIES CXX_STANDARD 17)

# emcc flags
if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	set_target_properties(Marbellous PROPERTIES LINK_FLAGS "-s EXPORTED_RUNTIME_METHODS='[ccall,cwrap]' -O3 -s ALLOW_MEMORY_GROWTH=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s ASSERTIONS=1")
	set_target_properties(Marbellous-debug PROPERTIES LINK_FLAGS "-fsanitize=undefined,address -g4 -s ASSERTIONS=1 -s TOTAL_MEMORY=278790144  -s EXPORTED_RUNTIME_METHODS='[ccall,cwrap]' -O3 -s ALLOW_MEMORY_GROWTH=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s ASSERTIONS=1 -s LLD_REPORT_UNDEFINED")
	set_target_properties(Marbellous-debug PROPERTIES COMPILE_FLAGS "-fsanitize=undefined,address -s ASSERTIONS=1 -g4")
endif()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Marbellous.js ${CMAKE_CURRENT_BINARY_DIR}/Marbellous.wasm DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/page/)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Marbellous-debug.js DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/page/
	RENAME Marbellous.js COMPONENT Marbellous-debug EXCLUDE_FROM_ALL)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Marbellous-debug.wasm ${CMAKE_CURRENT_BINARY_DIR}/Marbellous-debug.wasm.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/page/
	COMPONENT Marbellous-debug EXCLUDE_FROM_ALL)

install(DIRECTORY ${FRONTEND_DIR}/dist/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/page/ COMPONENT Marbellous-debug)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/page/ COMPONENT Marbellous-debug)
