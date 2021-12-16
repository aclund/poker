Compile    = g++
Libraries  =
DebugFlag  = -g 
#OPTFlag    = -O3
POKERFiles  := $(shell ls *.cpp)
POKERoFiles  = ${POKERFiles:.cpp=.o}
HEADERFiles  = $(shell ls *.h)

all: poker

poker:  $(POKERoFiles)
	$(Compile) -o poker  $(POKERoFiles) $(OPTFlag) $(DebugFlag) $(Libraries)

$(POKERoFiles): %.o: %.cpp $(HEADERFiles)
	$(Compile) $(OPTFlag) $(DebugFlag) -c $<

clean:
	rm -f *.o poker
	touch *
