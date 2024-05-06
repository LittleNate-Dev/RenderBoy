#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "uriparser::uriparser" for configuration "Release"
set_property(TARGET uriparser::uriparser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(uriparser::uriparser PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/uriparser.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS uriparser::uriparser )
list(APPEND _IMPORT_CHECK_FILES_FOR_uriparser::uriparser "${_IMPORT_PREFIX}/lib/uriparser.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
