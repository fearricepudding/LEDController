PROJECT(sockets)
cmake_minimum_required(VERSION 3.18)


include_directories(/usr/local/include /usr/include)
link_directories(/usr/local/lib /usr/lib)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD OFF)

find_package(Boost COMPONENTS thread system REQUIRED)


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
  
add_executable(server 
	src/LEDController.cpp
	src/LEDController.h
	src/animationEngine.cpp
	src/animationEngine.h
	src/ws2812-rpi.cpp
	src/ws2812-rpi.h
	src/ws2812-rpi-defines.h
	src/udpServer.h
	src/udpServer.cpp
	src/tcpServer.cpp
	src/tcpServer.h
)

target_link_libraries(server ${Boost_LIBRARIES})
