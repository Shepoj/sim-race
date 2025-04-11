#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libphysics::libphysics" for configuration "Debug"
set_property(TARGET libphysics::libphysics APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libphysics::libphysics PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/libphysics/Debug/liblibphysicsd.so"
  IMPORTED_SONAME_DEBUG "liblibphysicsd.so"
  )

list(APPEND _cmake_import_check_targets libphysics::libphysics )
list(APPEND _cmake_import_check_files_for_libphysics::libphysics "${_IMPORT_PREFIX}/libphysics/Debug/liblibphysicsd.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
