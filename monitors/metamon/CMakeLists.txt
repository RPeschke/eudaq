add_executable(MetaMon   src/MetaMon.cxx  )
set_property(TARGET MetaMon     PROPERTY PROJECT_LABEL "BinMetaMon") 
target_link_libraries(MetaMon   EUDAQ ${EUDAQ_THREADS_LIB} ${ADDITIONAL_LIBRARIES})

INSTALL(TARGETS MetaMon
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
