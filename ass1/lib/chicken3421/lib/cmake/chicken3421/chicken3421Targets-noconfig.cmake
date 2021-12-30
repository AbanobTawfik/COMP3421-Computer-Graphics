#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "chicken3421" for configuration ""
set_property(TARGET chicken3421 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(chicken3421 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libchicken3421.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS chicken3421 )
list(APPEND _IMPORT_CHECK_FILES_FOR_chicken3421 "${_IMPORT_PREFIX}/lib/libchicken3421.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
