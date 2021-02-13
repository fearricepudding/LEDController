PROJECT(sockets)
cmake_minimum_required(VERSION 3.10)


include_directories(/usr/local/include /usr/include)
link_directories(/usr/local/lib /usr/lib)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD OFF)

find_package(Boost COMPONENTS thread system REQUIRED)


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
  
add_executable(server 
	src/ws2812-rpi-defines.h

	src/LEDController.h
	src/animationEngine.h
	src/ws2812-rpi.h
	src/udpServer.h
	src/UdpThread.h
	src/commandHandler.h
	
	src/LEDController.cpp
	src/animationEngine.cpp
	src/ws2812-rpi.cpp
	src/udpServer.cpp
	src/UdpThread.cpp
	src/commandHandler.cpp

	src/TcpServer.cpp
	src/TcpServer.h
	src/TcpSession.cpp
	src/TcpSession.h
	src/TcpChannel.cpp
	src/TcpChannel.h
	src/udp_broadcaster.cpp
	src/udp_broadcaster.h

)

target_link_libraries(server ${Boost_LIBRARIES} jsoncpp)
