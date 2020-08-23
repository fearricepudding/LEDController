PROJECT(LEDController)
cmake_minimum_required(VERSION 2.8.11)

include_directories(include /usr/local/include /usr/include/python3.7m -I/usr/include/python3.7m)
link_directories(/usr/local/lib)


file(GLOB LEDController_src
	"src/*.cpp"
)
add_executable(LEDController ${LEDController_src})
#-lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm 
target_link_libraries(LEDController python3.7m crypt pthread dl util m)
