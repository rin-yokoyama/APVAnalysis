configure_file("this_apvanalysis.sh.in" this_apvanalysis.sh @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/this_apvanalysis.sh DESTINATION share/)

add_subdirectory(modulefiles)