TARGET = riks-server
LIBS = -lm -L./libs/libboost -lboost_system -lboost_chrono
CC = g++
CFLAGS = -g -Wall -I./libs/

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst source/%.cpp, bin/%.o, $(wildcard source/*.cpp))
HEADERS = $(wildcard header/*.h)

bin/%.o: source/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f bin/*.o
	-rm -f $(TARGET)