PROJECT(LEDController)
cmake_minimum_required(VERSION 2.8.11)

include_directories(include /usr/local/include /usr/include/python3.7m -I/usr/include/python3.7m)
link_directories(/usr/local/lib)

find_package(nlohmann_json 3.2.0 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

file(GLOB LEDController_src
	"src/*.cpp"
)
add_executable(LEDController ${LEDController_src})
#-lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm 
target_link_libraries(LEDController cppcms nlohmann_json::nlohmann_json booster dl pcre z pthread)
