enable_language(ASM)

file(GLOB_RECURSE LIBC_SOURCES *.cpp)

file(GLOB_RECURSE LIBC_HEADER *.h)

add_library(crt0 STATIC crt0.s)
target_link_libraries(crt0 PRIVATE NoCoverage)
#add_custom_command(
#	TARGET crt0
# 	COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_OBJECTS:crt0> ${CMAKE_INSTALL_PREFIX}/usr/lib/crt0.o
#)

add_library(crti STATIC crti.s)
target_link_libraries(crti PRIVATE NoCoverage)
#add_custom_command(
#    TARGET crti
#    COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_OBJECTS:crti> ${CMAKE_INSTALL_PREFIX}/usr/lib/crti.o
#)

add_library(crtn STATIC crtn.s)
target_link_libraries(crtn PRIVATE NoCoverage)
#add_custom_command(
#    TARGET crtn
#    COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_OBJECTS:crtn> ${CMAKE_INSTALL_PREFIX}/usr/lib/crtn.o
#)

set(CMAKE_CXX_FLAGS "-ffreestanding -Wno-builtin-declaration-mismatch")
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
