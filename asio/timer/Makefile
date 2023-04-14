SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))

CXX = g++
CXX_FLAGS = -g -W -Wall
LIB_FLAGS = -lpthread

all: build
build: $(OBJ)

%.o: %.cpp
	$(CXX) $< -o $@ $(CXX_FLAGS) $(LIB_FLAGS)

clean:
	rm -f $(OBJ)