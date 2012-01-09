GTEST           = ../gtest
GTEST_DIR	= $(GTEST)/current

CXX         = g++
LD	        = g++
CXXFLAGS    = -Wall  -I. -I$(GTEST_DIR)/include -Wno-deprecated -O2 -g -arch i386
LDFLAGS	    = -L$(GTEST) -arch i386

ALL_CC        = $(wildcard *.cc)
SOURCES_TESTS = $(filter-out Main.cc,$(ALL_CC))
SOURCES_CC    = $(filter-out %UnitTest.cc,$(ALL_CC))
SOURCES_C     = $(wildcard *.c)
OBJS          = $(SOURCES_CC:.cc=.o)
OBJS         += $(SOURCES_C:.c=.o)
TESTS_OBJS    = $(SOURCES_TESTS:.cc=.o)
EXE	          = simulation.x
TEST          = test.x

all: $(EXE)

$(EXE) : $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ 2>&1 | c++filt

.cc.o: .deps
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.c.o: .deps
	$(CC) $(CFLAGS) -c -o $@ $<

.deps : $(SOURCES_CC) $(SOURCES_C)
	$(CXX) $(CXXFLAGS) -MM $(SOURCES_CC) $(SOURCES_C) > $@

gtest:
	$(MAKE) -C $(GTEST)

gtest_main.o: $(GTEST_DIR)/src/gtest_main.cc
	$(CXX) $(CXXFLAGS) $(GTEST_DIR)/src/gtest_main.cc -c -o gtest_main.o

test: gtest gtest_main.o $(TESTS_OBJS)
	$(LD) $(LDFLAGS) $(TESTS_OBJS) gtest_main.o -o $(TEST) -lgtest 2>&1 | c++filt

-include .deps

clean :
	-rm -f *.o *~ *_old $(EXE) $(TEST)
	-rm -f simulation.x
	-rm -f core
	-rm -f *.vcd
	-rm -f .deps
	-rm -f *.tga
	-$(MAKE) -C ../gtest clean
