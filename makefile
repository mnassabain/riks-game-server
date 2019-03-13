TARGET = riks-server
LIBS = -lm -L./libs/libboost -L./libs/libsqlapi -lboost_system -lboost_chrono -lsqlapi -ldl -Wl,-rpath=./libs/libboost/ -Wl,-rpath=./libs/libsqlapi/
CC = g++ -std=c++11
CFLAGS = -g -Wall -I./libs/ -I./libs/sqlapi

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst source/%.cpp, bin/%.o, $(wildcard source/*.cpp))
HEADERS = $(wildcard header/*.h)

bin/%.o: source/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(CDRIVA) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f bin/*.o
	-rm -f $(TARGET)
