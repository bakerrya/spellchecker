CXX=g++
CXXFLAGS=-std=c++11 -Wall -pedantic

proj5.x: myspell.cpp hashtable.h hashtable.hpp
	$(CXX) $(CXXFLAGS) -g myspell.cpp -o proj5.x

clean:
	rm -f *.o *.x core.*


