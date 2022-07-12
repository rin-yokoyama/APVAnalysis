include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
set(LIB_NAME APVDecoder)
set(LIB_SOURCES
	source/APV8104Decoder.cpp
	source/APVDecodeManager.cpp
	source/APV7110Decoder.cpp
	source/APVGrapeDecodeManager.cpp
)
set(EXE_NAME apv_decoder)
add_library(${LIB_NAME} ${LIB_SOURCES})
add_executable(${EXE_NAME} source/APVDecoderMain.cpp)

target_link_libraries(${LIB_NAME} ${ROOT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} APVCore APVData)
target_link_libraries(${EXE_NAME} ${ROOT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${LIB_NAME} APVCore APVData)

install(TARGETS ${EXE_NAME}
		RUNTIME DESTINATION bin
		)
install(TARGETS ${LIB_NAME}
		LIBRARY DESTINATION lib
		)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
		DESTINATION include
		FILES_MATCHING PATTERN "*.h"
		)