SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))

CXX = g++
CXX_FLAGS = -g -W -Wall -O2
LIB_FLAGS = -lboost_system -lpthread

all: build
build: $(OBJ)

%.o: %.cpp
	$(CXX) $< -o $@ $(CXX_FLAGS) $(LIB_FLAGS)

clean:
	rm -f $(OBJ)