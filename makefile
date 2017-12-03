# Daniel Ramirez's moderately interesting final project makefile #
CXXFLAGS := -w -std=c++11
LDFLAGS := -L/use/local/lib -lXext -lm -Wl,-Bsymbolic-functions -lfltk_images -lfltk -lX11

all: main

debug: CXX += -g
debug: main

main: dgr2815_hw12.cpp
	$(CXX) $(CXXFLAGS) $(fltk-config --use-images --cxxflags) -o hw12 dgr2815_hw12.cpp $(LDFLAGS)

test: test.cpp
	$(CXX) $(CXXFLAGS) -o test test.cpp `fltk-config --ldflags`
	./test
clean: FORCE
	-rm -f shop.txt *.o hw12 test
FORCE:
