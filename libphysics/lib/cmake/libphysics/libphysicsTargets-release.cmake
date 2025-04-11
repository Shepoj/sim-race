#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libphysics::libphysics" for configuration "Release"
set_property(TARGET libphysics::libphysics APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libphysics::libphysics PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/libphysics/Release/liblibphysics.so"
  IMPORTED_SONAME_RELEASE "liblibphysics.so"
  )

list(APPEND _cmake_import_check_targets libphysics::libphysics )
list(APPEND _cmake_import_check_files_for_libphysics::libphysics "${_IMPORT_PREFIX}/libphysics/Release/liblibphysics.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
