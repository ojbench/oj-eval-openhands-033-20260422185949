CXX := g++
CXXFLAGS := -std=c++17 -O2 -pipe
LDFLAGS := 

all: code

code: 3.cpp src.hpp class-bint.hpp class-integer.hpp class-matrix.hpp
	$(CXX) $(CXXFLAGS) 3.cpp -o code $(LDFLAGS)

clean:
	rm -f code

.PHONY: all clean
