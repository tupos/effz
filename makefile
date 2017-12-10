UNAME:= $(shell uname)

ifeq ($(UNAME), Linux)
CXX = /usr/local/Packages/gcc-6.2.0/bin/g++
CC = /usr/local/Packages/gcc-6.2.0/bin/gcc
LIBDIR = /home/theo/ods/local
LDFLAGS = -L$(LIBDIR)/lib -Wl,-rpath,/usr/local/Packages/gcc-6.2.0/lib64
LDLIBS = -Wl,-Bstatic -larb -lflint -lmpfr -lgmp -lgslcblas -lsp_func -lgsl \
	 -Wl,-Bdynamic -lpthread -fopenmp

CXXFLAGS = -I$(LIBDIR)/include \
	   -I./include -g -c -std=gnu++11 -O2 -fopenmp
endif

ifeq ($(UNAME), Darwin)
CXX = g++
CC = gcc
LIBDIR = $$HOME/local
LDFLAGS = -L/usr/local/lib -L$(LIBDIR)/lib
LDLIBS =  -lm -lgsl -lsp_func -lflint -larb 
CXXFLAGS = -I/usr/local/include -I$(LIBDIR)/include \
	   -I./include -g -c -std=gnu++11 -O2
endif




SRCDIR = src
BUILDDIR = build
HEADERDIR = include

SOURCES = main.cpp grid.cpp integrate_nd.cpp utility.cpp

HEADERS = 


_OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS = $(patsubst %,$(BUILDDIR)/%,$(_OBJECTS))

DEPS = $(patsubst %,$(HEADERDIR)/%,$(HEADERS))

EXECUTABLE = eff_z


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) $(LDLIBS)

$(OBJECTS): | $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean all

clean:
	rm $(BUILDDIR)/*.o
