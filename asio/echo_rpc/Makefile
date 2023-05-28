all: client service
# MRPC_DIR = ../output
# INCLUDE_PATH = -I$(MRPC_DIR)/include/
# LIB = -L$(MRPC_DIR)/lib/ -lmuduo_base -lmuduo_net -lpthread -lprotobuf -lmrpc
# LIB = -L$(MRPC_DIR)/lib/ -lmuduo_base -lmuduo_net -lmrpc -lprotobuf -lmuduo_base -lmuduo_net -lpthread
# LIB = -L$(MRPC_DIR)/lib/ -lpthread -lmuduo_base -lmuduo_net -lmrpc -lprotobuf -lmuduo_base -lmuduo_net -lpthread
LIB = -lprotobuf -lpthread -lboost_system

CXX = g++
CXX_FLAGS = -g -pipe -W -Wall

client: client.cpp echo.pb.cc header.pb.cc
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(INCLUDE_PATH) $(LIB)

service: service.cpp echo.pb.cc header.pb.cc
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(INCLUDE_PATH) $(LIB)

%.pb.cc: %.proto
	protoc --proto_path=. --cpp_out=. $<

clean:
	rm -f client service