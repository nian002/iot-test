CC = aarch64-himix100-linux-gcc
CPP = aarch64-himix100-linux-g++
LINK = aarch64-himix100-linux-g++
CFLAGS = -g -Wall -Werror -Wextra -std=gnu99
CPPFLAGS = -g -Wall -Werror -Wextra
LIBS = -L./gtest-pub -lgtest -lgtest_main -L./opencv/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
INCS = -I./gtest-pub/include -I./opencv/include

C__SOURCES = $(wildcard *.c)
CPPSOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.c, %.o, $(C__SOURCES)) $(patsubst %.cpp, %.o, $(CPPSOURCES))
TARGET = unittest

first: all

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCS) -o $@ $< 

%.o: %.cpp
	$(CPP) -c $(CPPFLAGS) $(INCS) -o $@ $< 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK) $(CPPFLAGS) $(LIBS) -o $(TARGET) $(OBJECTS)
	rm $(OBJECTS)

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJECTS)
