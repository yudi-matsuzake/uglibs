set(GLAD_DIR ${CMAKE_SOURCE_DIR}/external/glad)

set(GLAD_SRC ${GLAD_DIR}/src/glad.c)
set(GLAD_HEADER 
	${GLAD_DIR}/include/glad/glad.h
	${GLAD_DIR}/include/KHR/khrplatform.h)

add_library(glad STATIC
	${GLAD_SRC}
	${GLAD_HEADER}
)

target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})

target_include_directories(glad 
	PUBLIC $<BUILD_INTERFACE:${GLAD_DIR}/include>
		$<INSTALL_INTERFACE:include/ug/glad/include>
)
