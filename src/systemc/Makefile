SYSTEMC		= /Library/SystemC/Current
TARGET_ARCH	= macosx
GTEST           = ../gtest
GTEST_DIR	= $(GTEST)/current

CXX         = g++
LD	    = g++
CFLAGS      = -arch i386
CXXFLAGS    = -Wall  -I. -isystem $(SYSTEMC)/include -I$(GTEST_DIR)/include -Wno-deprecated -O2 -g -arch i386
LDFLAGS	    =  -L$(SYSTEMC)/lib-$(TARGET_ARCH) -L$(GTEST) -arch i386

ALL_CPP     = $(wildcard *.cpp)
SOURCES_TESTS = $(filter-out Main.cpp,$(ALL_CPP))
SOURCES_CPP = $(filter-out %UnitTest.cpp,$(ALL_CPP))
SOURCES_C   = $(wildcard *.c)
OBJS        = $(SOURCES_CPP:.cpp=.o)
OBJS       += $(SOURCES_C:.c=.o)
TESTS_OBJS  = $(SOURCES_TESTS:.cpp=.o)
EXE	    = simulation.x
TEST        = test.x

all: $(EXE)

$(EXE) : $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@  -lsystemc 2>&1 | c++filt

.cpp.o: .deps
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o: .deps
	$(CC) $(CFLAGS) -c -o $@ $<

.deps : $(SOURCES_CPP) $(SOURCES_C)
	$(CXX) $(CXXFLAGS) -MM $(SOURCES_CPP) $(SOURCES_C) > $@

gtest:
	$(MAKE) -C $(GTEST)

gtest_main.o: $(GTEST_DIR)/src/gtest_main.cc
	$(CXX) $(CXXFLAGS) $(GTEST_DIR)/src/gtest_main.cc -c -o gtest_main.o

test: gtest gtest_main.o $(TESTS_OBJS)
	$(LD) $(LDFLAGS) $(TESTS_OBJS) gtest_main.o -o $(TEST) -lsystemc -lgtest 2>&1 | c++filt

-include .deps

clean :
	-rm -f *.o *~ *_old $(EXE) $(TEST)
	-rm -f simulation.x
	-rm -f core
	-rm -f *.vcd
	-rm -f .deps
	-rm -f *.tga
	-$(MAKE) -C ../gtest clean
