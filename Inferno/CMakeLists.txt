set(INFERNO_SOURCES
	Drivers/COM/COM.cpp
	Drivers/IO/IO.cpp
	Drivers/GDT/Load.s
	main.cpp
)

add_executable(Inferno ${INFERNO_SOURCES})
