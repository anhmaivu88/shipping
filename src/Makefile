CXXFLAGS = -Wall -g -std=c++11

OBJECTS = Segment.o Instance.o Engine.o

default:	test1 example

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test2: test2.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h
Engine.o: Engine.cpp Engine.h
Segment.o: Segment.cpp Segment.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
test2.o: test2.cpp
Statistics.o: Statistics.cpp Statistics.h
