#MAKEFILE

SRCS       := $(wildcard ./src/*.cpp)
OBJS       := $(patsubst ./src/%.cpp, ./obj/%.o, $(SRCS))
################################################################################

TARGET      = FM

CXX         = g++ 
CXXFLAGS    = -O3 -Wall -g -pthread
LIB         = $(DEFLEF_LIB) $(LASPACK_LIB) $(FLUTE_LIB)
INCLUDE     = $(DEFLEF_INC) $(LASPACK_INC) $(FLUTE_INC)

# #DEFLEF

# DEFLEF_DIR   = /ift/home/m112pychen/Desktop/library/DEFLEF
# # DEFLEF_DIR   = /ift/home/m112pychen/Desktop/library/DEFLEF5.8
# DEFLEF_INC   = -I $(DEFLEF_DIR)/lef/include -I $(DEFLEF_DIR)/def/include
# DEFLEF_LIB   = -L $(DEFLEF_DIR)/lef/lib -llef -L $(DEFLEF_DIR)/def/lib -ldef

# #LASPACK

# LASPACK_DIR  = /ift/home/m112pychen/Desktop/library/laspack
# LASPACK_INC  = -I $(LASPACK_DIR)/include
# LASPACK_LIB  = -L $(LASPACK_DIR)/lib -lxc -llaspack

# #FLUTE

# FLUTE_DIR    = /ift/home/m112pychen/Desktop/library/flute-3.1
# FLUTE_INC    = -I $(FLUTE_DIR)/include
# FLUTE_LIB    = -L $(FLUTE_DIR)/lib -lflute


#################################################################################

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIB) -o $(TARGET)

./obj/%.o: ./src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(TARGET) ./obj/*.o core *~
