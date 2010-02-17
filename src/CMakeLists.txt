include_directories(../include)

file(GLOB HEADERS ../include/*.h)
file(GLOB SRCS *.cpp)
#==================================================


add_library (

	bioamp
	SHARED
	${SRCS}
	${HEADERS}

)

target_link_libraries( bioamp usb-1.0 )
INSTALL(TARGETS bioamp DESTINATION ${CMAKE_INSTALL_LIBDIR})
