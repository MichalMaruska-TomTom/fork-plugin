cmake_minimum_required(VERSION 3.13)

# /usr/share/cmake-3.13/Modules/FindPkgConfig.cmake
# set(PKG_CONFIG_EXECUTABLE "$ENV{PKG_CONFIG}" CACHE FILEPATH "pkg-config executable")
# mark_as_advanced(PKG_CONFIG_EXECUTABLE)
find_package(PkgConfig)

set(CMAKE_DEBUG_TARGET_PROPERTIES
  #INCLUDE_DIRECTORIES
  # COMPILE_DEFINITIONS
  COMPILE_OPTIONS
  AUTOUIC_OPTIONS
  POSITION_INDEPENDENT_CODE
  )
# not GLOB_RECURSE
# file(GLOB sources "*.cpp"  "*.c")
set(sources
  configure.cpp
  debug.cpp
  event_ops.cpp
  fork.cpp
  machine.cpp
  history.cpp
  )

add_library(fork MODULE ${sources})

target_compile_definitions(fork PRIVATE VERSION_STRING="git-local-2")
target_compile_options(fork PRIVATE "-g")
set_property(TARGET fork PROPERTY NO_SONAME)
# set_property(TARGET fork PROPERTY OUTPUT_NAME fork.so)
set_target_properties(fork PROPERTIES PREFIX "")
# SHARED

pkg_search_module(xorg-server REQUIRED xorg-server
  IMPORTED_TARGET GLOBAL)
# /usr/lib/x86_64-linux-gnu/pkgconfig/xorg-server.pc
# or SDK?
#target_include_directories(fork SYSTEM PUBLIC ${xorg-server_INCLUDE_DIRS})

target_link_libraries(fork PkgConfig::xorg-server) # ${xorg-server_LIBRARIES}
# PROJECT_BINARY_DIR
# ${PROJECT_SOURCE_DIR}/
include_directories(../include/)

# set(CMAKE_MY_NAME "" CACHE STRING "Flags used by the compiler during all build types.")

pkg_get_variable(moduledir xorg-server moduledir)
pkg_get_variable(includedir xorg-server sdkdir)


message(STATUS "Building Fork & install in " ${moduledir})
message(STATUS "USing Xorg headers from " ${includedir})

# $<INSTALL_PREFIX>
install(FILES ../include/fork_requests.h
  #PUBLIC_HEADER
  DESTINATION ${includedir}
  )

install(EXPORT EXPORT_module DESTINATION ${moduledir}/)

install(TARGETS fork
  EXPORT EXPORT_module
  LIBRARY
  # OBJECTS
  DESTINATION ${moduledir}/
  RENAME fork.so
  # CMAKE_INSTALL_PREFIX CMAKE_INSTALL_LIBDIR
  )
