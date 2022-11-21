set(LIBC_SOURCES
	assert.cpp
	stdlib.cpp
)

set(LIBC_HEADER
	assert.h
	bits/pthread_integration.h
	ctype.h
	errno.h
	fcntl.h
	float.h
	limits.h
	pthread/pthread.h
	pthreadtypes.h
	stdarg.h
	stddef.h
	stdint.h
)

add_link_options(-nostdlib)
add_library(C SHARED ${LIBC_SOURCES})

SET_TARGET_PROPERTIES (C PROPERTIES PUBLIC_HEADER "${LIBC_HEADER}")

set(LIBRARY_VERSION_MAJOR 0)
set(LIBRARY_VERSION_STRING 0.1)

install (
	TARGETS C
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
