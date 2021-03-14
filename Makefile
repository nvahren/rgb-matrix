CXX = g++
CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS = $(CFLAGS)
SOURCES=life.cc ants.cc
OBJECTS=life.o ants.o
BINARIES=life ants

RGB_LIB_DISTRIBUTION=matrix
	RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
	RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
	RGB_LIBRARY_NAME=rgbmatrix
	RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
	LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

all: life ants

debug : $(SOURCES) $(RGB_LIBRARY)
	$(CXX) -o life.o -I. -I$(RGB_INCDIR) $(CXXFLAGS) $(LDFLAGS) -c life.cc -DDEBUG
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o life $(LDFLAGS)
	$(CXX) -o ants.o -I. -I$(RGB_INCDIR) $(CXXFLAGS) $(LDFLAGS) -c ants.cc -DDEBUG
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o ants $(LDFLAGS)

life : life.o $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) life.o -o life $(LDFLAGS)

life.o:
	$(CXX) -o life.o -I. -I$(RGB_INCDIR) $(CXXFLAGS) $(LDFLAGS) -c life.cc

ants : ants.o $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) ants.o -o ants $(LDFLAGS)

ants.o:
	$(CXX) -o ants.o -I. -I$(RGB_INCDIR) $(CXXFLAGS) $(LDFLAGS) -c ants.cc

clean:
	rm -f $(OBJECTS) $(BINARIES)

.PHONY: clean
