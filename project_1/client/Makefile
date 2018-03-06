TARGETS = udp_handler.cc
CC = g++

CFLAGS += -std=c++11 -g -lpthread
LDLIBS += -lnsl -lpthread


OBJ = 

INC_DIR = ../util/

OBJECTS := $(TARGETS:%.cc=%.o)

all: $(OBJECTS)

$(OBJECTS): %.o : %.cc
	g++ -std=c++11 -I$(INC_DIR) -c $< -o $@ -lpthread

clean:
	rm *.o
