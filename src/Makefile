CXXFLAGS = -Wall -g

OBJECTS = Instance.o

default:	test1 example

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h Segment.h Location.h Connectivity.h Fleet.h Entity.h Statistics.h Mile.h Error.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
