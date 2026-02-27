export I_MPI_CXX=icx

CXX		    := mpiicpc
CXXFLAGS 	:= -std=c++17 -Wall -Wextra -g
LDFLAGS		:= -lstdc++

NAME		:= $(shell basename $(CURDIR))
SOURCES		:= main.cpp
OBJECTS		:= $(SOURCES:.cpp=.o)
OUTPUT		:= $(NAME)

$(OUTPUT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $(OUTPUT)

$(OBJECTS): $(SOURCES)
	$(CXX) $(CXXFLAGS) -c $(SOURCES) -o $(OBJECTS)

run:
	mpirun -np 1 ./$(OUTPUT)

clean:
	rm -rf $(OUTPUT) *.o

.PHONY: run clean