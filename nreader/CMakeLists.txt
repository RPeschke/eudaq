# CMake script to build library providing native reader Marlin processor for data conversion into LCIO
# and reconstruction using EUTelescope analysis framework
SET( libname NativeReader )


#FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES REQUIRED ILCTEST)
FIND_PACKAGE( ILCUTIL HINTS "C:/Program Files (x86)/ILCUTIL")
# load default settings from ILCSOFT_CMAKE_MODULES


#FIND_PACKAGE( Marlin 1.0 REQUIRED HINTS "C:/Program Files (x86)/Marlin")
#FIND_PACKAGE( MarlinUtil 1.2 REQUIRED )

LINK_LIBRARIES( EUDAQ )




### LIBRARY #################################################################

ADD_DEFINITIONS("-DUSE_EUDAQ")
ADD_DEFINITIONS("-DUSE_MARLIN")
ADD_DEFINITIONS("-DUSE_GEAR")
ADD_DEFINITIONS("-DUSE_MARLINUTIL")

# ---------------------------------------------------------------------------

# include directories
INCLUDE_DIRECTORIES( ./include )

AUX_SOURCE_DIRECTORY( ./src library_sources )
ADD_LIBRARY( ${libname} SHARED ${library_sources} )

INSTALL(TARGETS ${libname}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

