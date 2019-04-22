TARGET = riks-server
LIBS = -lm -L./libs/libboost -L./libs/libsqlite -lboost_system -lboost_chrono -lpthread -lsqlite3 -ldl -Wl,-rpath=./libs/libboost/
CC = g++ -std=c++11
CFLAGS = -g -Wall -Wno-deprecated-declarations -I./libs/
DOCKER_APP = ms/riks-server
DOCKER_CONTAINER = riks-server
PORT = 9002

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

# DOCKER TASKS
docker-build:
	docker build -t $(DOCKER_APP) .

docker-run:
	docker run --rm -ti -p $(PORT):$(PORT) --name="$(DOCKER_CONTAINER)" $(DOCKER_APP)

docker-stop:
	docker stop $(DOCKER_CONTAINER)

clean:
	-rm -f bin/*.o
	-rm -f $(TARGET)
