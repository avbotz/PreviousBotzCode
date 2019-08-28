# 
# Makefile for AQUASTORM AUV software stack.
#

CXX = g++
SRC = src
BIN = bin
EXE = aquastorm
BUILD = build
THIRDPARTY = ./3rdparty
CXX_FLAGS = -ggdb -c -std=c++14 -Iinclude 

# Flags for 3rdparty libraries.
OPENCV_CFLAGS = `pkg-config --cflags opencv`
OPENCV_LFLAGS = `pkg-config --libs opencv` -pthread

FLYCAP = $(THIRDPARTY)/flycapture
FLYCAP_CFLAGS = -I$(SRC) -I$(FLYCAP)/include
FLYCAP_LFLAGS = -L$(FLYCAP)/lib -lflycapture -Wl,-rpath=$(FLYCAP)/lib

ZEROMQ_CFLAGS = 
ZEROMQ_LFLAGS = -lzmq

ARMADILLO = $(THIRDPARTY)/armadillo
ARMADILLO_CFLAGS = -I$(SRC) -I$(ARMADILLO)/include 
ARMADILLO_LFLAGS = -L$(ARMADILLO)/lib -larmadillo -Wl,-rpath=$(ARMADILLO)/lib

# Objects and specific compile flags.
COMMON = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC)/common/*.cpp))
COMMON_CFLAGS = 
COMMON_LFLAGS = 

VISION = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC)/vision/*.cpp))
VISION_CFLAGS = $(OPENCV_CFLAGS)
VISION_LFLAGS = $(OPENCV_LFLAGS)

MODELING = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC)/modeling/*.cpp)) 
MODELING_CFLAGS = $(ARMADILLO_CFLAGS)
MODELING_LFLAGS = $(ARMADILLO_LFLAGS)

MISSION = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC)/mission/*.cpp)) $(COMMON) $(VISION)
MISSION_CFLAGS = $(OPENCV_CFLAGS)
MISSION_LFLAGS = $(OPENCV_LFLAGS)

CAPTURE = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC)/capture/*.cpp)) $(COMMON)
CAPTURE_CFLAGS = $(OPENCV_CFLAGS) $(FLYCAP_CFLAGS)
CAPTURE_LFLAGS = $(OPENCV_LFLAGS) $(FLYCAP_LFLAGS)

SERVER = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC)/server/*.cpp)) $(COMMON)
SERVER_CFLAGS = $(OPENCV_CFLAGS) $(ZEROMQ_CFLAGS)
SERVER_LFLAGS = $(OPENCV_LFLAGS) $(ZEROMQ_LFLAGS)

CONTROL = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC)/control/*.cpp)) $(COMMON)
CONTROL_CFLAGS =
CONTROL_LFLAGS = 

INTERFACE = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC)/interface/*.cpp)) $(COMMON) $(CAPTURE) $(SERVER) $(CONTROL) $(MISSION) $(MODELING)
INTERFACE_CFLAGS = $(OPENCV_CFLAGS)
INTERFACE_LFLAGS = $(OPENCV_LFLAGS)

# Program dependencies.
COMMON_DEP = $(COMMON:%.o=%.d)
VISION_DEP = $(VISION:%.o=%.d)
MODELING_DEP = $(MODELING:%.o=%.d)
MISSION_DEP = $(MISSION:%.o=%.d)
CAPTURE_DEP = $(CAPTURE:%.o=%.d)
SERVER_DEP = $(SEVER:%.o=%.d)
CONTROL_DEP = $(CONTROL:%.o=%.d)
INTERFACE_DEP = $(INTERFACE:%.o=%.d)

# Default target named after binary.
# $(BIN) : $(BUILD)/$(BIN)

# Actual target of the binary which depends on all .o files.
# $(BUILD)/$(BIN) : $(INTERFACE)
	#$(CXX) $(CXX_FLAGS) $^ -o $@

all: $(BIN)/$(EXE)

$(BIN)/$(EXE): $(INTERFACE)
	$(CXX) $^ $(FLYCAP_LFLAGS) $(OPENCV_LFLAGS) $(ARMADILLO_LFLAGS) $(INTERFACE_LFLAGS) $(SERVER_LFLAGS) -o $@

# Include all dependencies.
DEP = $(COMMON_DEP) $(VISION_DEP) $(MODELING_DEP) $(MISSION_DEP) $(CAPTURE_DEP) $(SERVER_DEP) $(CONTROL_DEP) $(INTERFACE_DEP)
-include $(DEP)

# Build target for each object file.
$(BUILD)/common/%.o : $(SRC)/common/%.cpp
	$(CXX) $(CXX_FLAGS) $(COMMON_CFLAGS) -MMD $< -o $@

$(BUILD)/vision/%.o : $(SRC)/vision/%.cpp
	$(CXX) $(CXX_FLAGS) $(VISION_CFLAGS) -MMD $< -o $@

$(BUILD)/modeling/%.o : $(SRC)/modeling/%.cpp
	$(CXX) $(CXX_FLAGS) $(MODELING_CFLAGS) -MMD $< -o $@

$(BUILD)/mission/%.o : $(SRC)/mission/%.cpp
	$(CXX) $(CXX_FLAGS) $(MISSION_CFLAGS) -MMD $< -o $@

$(BUILD)/capture/%.o : $(SRC)/capture/%.cpp
	$(CXX) $(CXX_FLAGS) $(CAPTURE_CFLAGS) -MMD $< -o $@

$(BUILD)/server/%.o : $(SRC)/server/%.cpp
	$(CXX) $(CXX_FLAGS) $(SERVER_CFLAGS) -MMD $< -o $@

$(BUILD)/control/%.o : $(SRC)/control/%.cpp
	$(CXX) $(CXX_FLAGS) $(CONTROL_CFLAGS) -MMD $< -o $@

$(BUILD)/interface/%.o : $(SRC)/interface/%.cpp
	$(CXX) $(CXX_FLAGS) $(INTERFACE_CFLAGS) -MMD $< -o $@

# Clean project.
clean:
	rm -f $(BIN)/*
	rm -f $(BUILD)/**/*
