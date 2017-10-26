CXX=g++
#CXXFLAGS= -std=c++11

all: MAIN MAIN_debug

MAIN: main.cpp 
	$(CXX) -std=c++11 $^ -o alg

MAIN_debug: main.cpp 
	$(CXX) -g -std=c++11 $^ -o alg_debug

clean:
	rm -f alg alg_debug
	rm -f *.o *.gch
