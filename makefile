# Daniel Ramirez's moderately interesting fltk gui makefile #
CXXFLAGS := -w -std=c++11

all: main

debug: CXX += -g
debug: main

main: dgr2815_hw6.cpp
	$(CXX) $(CXXFLAGS) -o hw6 dgr2815_hw6.cpp

test: test.cpp
	$(CXX) $(CXXFLAGS) -o test test.cpp
	./menu
clean: FORCE
	-rm -f *.o hw6 test
FORCE:
