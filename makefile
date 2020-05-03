LD = g++
CXX = g++

CFLAGS = -Wall -fexceptions -g -lpthread -lcppcms
INC = -I /usr/local/include -I include
LIBDIR = -L /usr/local/include
LIB = /usr/local/lib/libbooster.a /usr/local/lib/libbooster.so /usr/local/lib/libcppcms.a /usr/local/lib/libcppcms.so /usr/local/lib/libpigpio.so
LDFLAGS = -lpthread -lcppcms -ldl -lpcre -licuuc -licui18n -lpigpio
OBJDIR = obj
OUT = bin/LEDController

before_build:
	test -d bin || mkdir -p bin
	test -d obj || mkdir -p obj

after_build:

build: before_build out_build after_build

$(OBJDIR)/main.o: src/main.cpp
	$(CXX) $(CFLAGS) $(INC) -c src/main.cpp -o $(OBJDIR)/main.o
	$(CXX) $(CFLAGS) $(INC) -c src/LEDController.cpp -o $(OBJDIR)/LEDController.o

out_build: before_build $(OBJDIR)/main.o
	$(LD) $(LIBDIR) -o $(OUT) $(OBJDIR)/main.o $(OBJDIR)/LEDController.o $(LDFLAGS) $(LIB)

build: before_build out_build

clean: 
	rm -dfr bin
	rm -dfr obj
