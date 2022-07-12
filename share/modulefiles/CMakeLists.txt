configure_file("APVAnalysis.in" APVAnalysis @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/APVAnalysis DESTINATION share/modulefiles)