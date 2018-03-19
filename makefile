UNAME:= $(shell uname)

ifeq ($(UNAME), Linux)
CXX = /usr/local/Packages/gcc-6.2.0/bin/g++
CC = /usr/local/Packages/gcc-6.2.0/bin/gcc
LIBDIR = /home/theo/ods/local
LDFLAGS = -L$(LIBDIR)/lib -Wl,-rpath,/usr/local/Packages/gcc-6.2.0/lib64
LDLIBS = -Wl,-Bstatic -larb -lflint -lmpfr -lgmp -lgslcblas \
		 -lsp_func -lgsl -Wl,-Bdynamic -lpthread -fopenmp

CXXFLAGS = -I$(LIBDIR)/include \
	   -I./include -g -c -std=c++14 -O2 -fopenmp
endif

ifeq ($(UNAME), Darwin)
CXX = g++
CC = gcc
PYTHONCFLAGS = -I/usr/local/Cellar/python3/3.6.4_4/Frameworks/Python.framework/Versions/3.6/include/python3.6m \
			   -fno-common -dynamic -DNDEBUG -g -fwrapv
PYTHONLDFLAGS = -L/usr/local/opt/python3/Frameworks/Python.framework/Versions/3.6/lib/python3.6/config-3.6m-darwin \
				-lpython3.6m -ldl -framework CoreFoundation
LIBDIR = $$HOME/local
LDFLAGS = -L/usr/local/lib -L$(LIBDIR)/lib $(PYTHONLDFLAGS)
LDLIBS =  -lm -lgsl -lsp_func -lflint -larb -ltbb
CXXFLAGS = -I/usr/local/include -I$(LIBDIR)/include \
		   -I./include $(PYTHONCFLAGS) -g -c -std=c++14 -O2 -MMD #-Wall
endif




SRCDIR = src
BUILDDIR = build
HEADERDIR = include

SOURCES = main.cpp grid.cpp integrate_nd.cpp effz_utility.cpp\
		  effz_spec_func.cpp effz_zeroth_order.cpp\
		  effz_atomic_data.cpp effz_zeroth_order_python.cpp\
		  monte_complex.cpp effz_exceptions.cpp\
		  effz_python_utility.cpp effz_main_loop_functions.cpp\
		  effz_user_input.cpp effz_result.cpp
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
	rm $(BUILDDIR)/*.o $(BUILDDIR)/*.d

-include $(BUILDDIR)/*.d
