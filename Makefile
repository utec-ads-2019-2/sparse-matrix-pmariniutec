OBJECTS = main.cpp mocker/mocker.h mocker/mocker.cpp tester/tester.h tester/tester.cpp 
STD = -std=c++17
MYFLAGS = -g -Wall
CXX = g++
EXEC = main

all: $(OBJECTS)
	$(CXX) $(STD) $(MYFLAGS) $(OBJECTS) -o $(EXEC)

clean: 
	rm $(EXEC)
