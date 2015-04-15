
SET(Example_name ExampleSCT_Producer)
INCLUDE_DIRECTORIES( . inc )

FILE(GLOB SRCFILES_CXX src/*.cxx)
FILE(GLOB SRCFILES_CC src/*.cc)
add_executable(${Example_name} ${SRCFILES_CXX} ${SRCFILES_CC})


target_link_libraries(${Example_name}   EUDAQ )


INSTALL(TARGETS ${Example_name} 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
