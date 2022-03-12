cmake_minimum_required(VERSION 3.20)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")


project(gpp)



file(GLOB PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB gpp_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

message(${gpp_files})

add_executable(gpp_e ${gpp_files})

if (UNIT_TEST)
  message("TESTING ENABLED")
  target_compile_options(gpp_e PUBLIC -g -Wall)
else()
  target_compile_options(gpp_e PRIVATE -g -Wall)
endif()



list(FILTER gpp_files EXCLUDE REGEX "main.c")

add_library(gpp SHARED ${gpp_files})
add_library(gpp_static STATIC ${gpp_files})

target_compile_options(gpp PUBLIC -fPIC)
target_compile_options(gpp_static PUBLIC -fPIC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  gpp
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(
  gpp_static
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(
  gpp_e
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


    include(FetchContent)



include(gpp_deps)


init_gpp_deps(gpp)
init_gpp_deps(gpp_static)
init_gpp_deps(gpp_e)

if (UNIT_TEST)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
