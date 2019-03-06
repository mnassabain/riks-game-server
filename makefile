TARGET = riks-server
LIBS = -lm
CC = g++
CFLAGS = -g -Wall

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