CXX = g++
CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS = $(CFLAGS)
OBJECTS=life.o
BINARIES=life

RGB_LIB_DISTRIBUTION=matrix
	RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
	RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
	RGB_LIBRARY_NAME=rgbmatrix
	RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
	LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

all: life.o

life : $(OBJECTS) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

life.o:
	$(CXX) -o life.o -I. -I$(RGB_INCDIR) $(CXXFLAGS) $(LDFLAGS) -c life.cc

clean:
	rm -f $(OBJECTS) $(BINARIES)
