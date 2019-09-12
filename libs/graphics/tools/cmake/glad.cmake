set(GLAD_DIR ${CMAKE_SOURCE_DIR}/external/glad)

add_library(ug_glad STATIC
	${GLAD_DIR}/src/glad.c

	${GLAD_DIR}/include/glad/glad.h
	${GLAD_DIR}/include/KHR/khrplatform.h
)

target_include_directories(ug_glad 
	PUBLIC $<BUILD_INTERFACE:${GLAD_DIR}/include>
		$<INSTALL_INTERFACE:include/ug>
)
