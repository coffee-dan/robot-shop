# Daniel Ramirez's moderately interesting final project makefile #
CXXFLAGS := -w -std=c++11

all: main

debug: CXX += -g
debug: main

main: dgr2815_hw10.cpp
	$(CXX) $(CXXFLAGS) -o hw10 dgr2815_hw10.cpp

test: test.cpp
	$(CXX) $(CXXFLAGS) -o test test.cpp
	./test
clean: FORCE
	-rm -f shop.txt *.o hw10 test
FORCE:
