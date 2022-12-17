include compiler.spec
include dependencies.make

OBJECTS  = $(shell ls *cpp | sed 's/cpp/o/')

#-------------------------------------------------------------------------------
#   Rule for building all programs

all: $(PROGRAMS)
	make $<

$(PROGRAMS): %:
	$(COMPILE) $(FLAGS) $(LIBRARIES) $^ -o $@

$(OBJECTS): %.o: %.cpp
	$(COMPILE) $(FLAGS) -c $<

#-------------------------------------------------------------------------------
#   Rule for cleaning

clean:
	rm -f *.o

clean_all: clean
	rm -f $(PROGRAMS)
	touch *

#-------------------------------------------------------------------------------
#   Rule for compiler

opt: clean
	rm compiler.spec; ln -s compiler.opt compiler.spec

debug: clean
	rm compiler.spec; ln -s compiler.debug compiler.spec
