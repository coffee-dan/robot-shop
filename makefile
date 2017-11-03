# Daniel Ramirez's moderately interesting fltk gui makefile #
# depreciated #
CXXFLAGS := -w -std=c++11

all: main

debug: CXX += -g
debug: main

main: dgr2815_hw7.cpp
	$(CXX) $(CXXFLAGS) -o hw7 dgr2815_hw6.cpp

test: test.cpp
	$(CXX) $(CXXFLAGS) -o test test.cpp
	./test
clean: FORCE
	-rm -f *.o hw7 test
FORCE:
