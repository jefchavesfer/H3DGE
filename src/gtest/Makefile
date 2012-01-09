GTEST_DIR = current
CXXFLAGS  = -I$(GTEST_DIR)/include -I$(GTEST_DIR) -arch i386
ARFLAGS   = -rv
LIB = libgtest.a
OBJ = gtest-all.o
SRC = $(GTEST_DIR)/src/gtest-all.cc

all: $(LIB)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $(LIB) $(OBJ)

clean:
	-rm -f *.o *~ *.a
