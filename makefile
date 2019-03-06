TARGET = main
LIBS = -lm
CC = g++
CFLAGS = #-g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst source/%.cpp, source/%.o, $(wildcard source/*.cpp))
HEADERS = $(wildcard header/*.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f source/*.o
	-rm -f $(TARGET)