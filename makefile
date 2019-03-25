TARGET = riks-server
LIBS = -lm -L./libs/libboost -L./libs/libsqlite -lboost_system -lboost_chrono -lsqlite3 -ldl -Wl,-rpath=./libs/libboost/
CC = g++ -std=c++11
CFLAGS = -g -Wall -Wno-deprecated-declarations -I./libs/

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
