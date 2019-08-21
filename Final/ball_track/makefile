SRC=ball_track.cc
CXX=g++
CFLAGS=-Wall -g -std=c++11
HEADERS=
LIBS=$(shell pkg-config --libs opencv)
CXXFLAGS = $(shell pkg-config --cflags opencv)
all: ball

ball: $(SRC) $(LIBS)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(SRC) -o ball_tracker $(LIBS)  
 
clean:
	rm -f *.o *~
