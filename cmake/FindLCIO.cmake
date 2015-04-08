# - Try to find LCIO used in native reader library for exporting RAW to LCIO format needed for reconstruction
# Once done this will define
#  LCIO_FOUND - System has LCIO
#  LCIO_INCLUDE_DIRS - The LCIO include directories
#  LCIO_LIBRARIES - The libraries needed to use LCIO
#  LCIO_DEFINITIONS - Compiler switches required for using LCIO
IF(WIN32)
  find_path(LCIO_INCLUDE_DIR lcio.h
    HINTS "${LCIO}/include" "$ENV{LCIO}/include" "${PROJECT_SOURCE_DIR}/extern/lcio/include")

  find_library(LCIO_LIBRARY NAMES lcio
    HINTS "${LCIO}/lib" "$ENV{LCIO}/lib" "${PROJECT_SOURCE_DIR}/extern/lcio/release/lib")

  find_library(LCIO_LIBRARY_DEBUG NAMES lcio
    HINTS "${LCIO}/lib" "$ENV{LCIO}/lib" "${PROJECT_SOURCE_DIR}/extern/lcio/debug/lib")

  set(LCIO_LIBRARIES_DEBUG ${LCIO_LIBRARY_DEBUG} )
  set(LCIO_LIBRARIES ${LCIO_LIBRARY} )
ELSE(WIN32)
  find_path(LCIO_INCLUDE_DIR lcio.h
    HINTS "${LCIO}/include" "$ENV{LCIO}/include")

  find_library(LCIO_LIBRARY NAMES lcio
    HINTS "${LCIO}/lib" "$ENV{LCIO}/lib")
  
  set(LCIO_LIBRARIES_DEBUG ${LCIO_LIBRARY} )
  set(LCIO_LIBRARIES ${LCIO_LIBRARY} )
ENDIF(WIN32)



set(LCIO_INCLUDE_DIRS ${LCIO_INCLUDE_DIR} )
set(LCIO_DEFINITIONS "-DUSE_LCIO" )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LCIO_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LCIO  DEFAULT_MSG
                                  LCIO_LIBRARY LCIO_INCLUDE_DIR)

mark_as_advanced(LCIO_INCLUDE_DIR LCIO_LIBRARY )