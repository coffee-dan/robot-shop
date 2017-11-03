# Daniel Ramirez's moderately interesting fltk gui makefile #
CXXFLAGS := -w -std=c++11

all: main

debug: CXX += -g
debug: main

main: dgr2815_hw8.cpp
	$(CXX) $(CXXFLAGS) -o hw8 dgr2815_hw8.cpp

test: test.cpp
	$(CXX) $(CXXFLAGS) -o test test.cpp
	./test
clean: FORCE
	-rm -f *.o hw8 test
FORCE:
