CXX = g++
CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS = $(CFLAGS)
OBJECTS=rgb-matrix.o
BINARIES=rgb-matrix

RGB_LIB_DISTRIBUTION=matrix
	RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
	RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
	RGB_LIBRARY_NAME=rgbmatrix
	RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
	LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

all: rgb-matrix.o

rgb-matrix : $(OBJECTS) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

rgb-matrix.o: rgb-matrix.cc
	$(CXX) -o rgb-matrix.o -I. -I$(RGB_INCDIR)$(CXXFLAGS) $(LDFLAGS) -c rgb-matrix.cc

clean:
	rm -f $(OBJECTS) $(BINARIES)
