cmake_minimum_required(VERSION 3.20)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/inputs DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/context.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules")


project(gpp_test)



file(GLOB PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

message(${source_files})

add_executable(gpp_test ${source_files})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

include(FetchContent)

include(gpp_deps)

target_link_libraries(gpp_test PRIVATE gpp_static)

init_gpp_deps(gpp_test)
