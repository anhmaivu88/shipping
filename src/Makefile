CXXFLAGS = -Wall -g -std=c++0x

OBJECTS = Segment.o Instance.o Engine.o Statistics.o Connectivity.o 

default:	test1 example UnitedStatesCities

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test2: test2.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

UnitedStatesCities: UnitedStatesCities.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o test2 test2.o example example.o UnitedStatesCities UnitedStatesCities.o $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h
Engine.o: Engine.cpp Engine.h
Segment.o: Segment.cpp Segment.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
test2.o: test2.cpp
UnitedStatesCities.o: UnitedStatesCities.cpp
Statistics.o: Statistics.cpp Statistics.h
Connectivity.o: Connectivity.cpp Connectivity.h
