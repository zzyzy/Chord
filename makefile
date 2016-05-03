EXECUTABLE_NAME = Chord
SOURCE_DIR = src
INCLUDE_DIR = include
DEBUG_DIR = bin/debug
RELEASE_DIR = bin/release
DEBUG_OBJ_DIR = obj/debug
RELEASE_OBJ_DIR = obj/release
DEBUG_PATH = $(DEBUG_DIR)/$(EXECUTABLE_NAME)
RELEASE_PATH = $(RELEASE_DIR)/$(EXECUTABLE_NAME)
CPP_FILES = $(wildcard $(SOURCE_DIR)/*.cpp)
DEBUG_OBJ_FILES = $(addprefix $(DEBUG_OBJ_DIR)/, $(notdir $(CPP_FILES:.cpp=.o)))
RELEASE_OBJ_FILES = $(addprefix $(RELEASE_OBJ_DIR)/, $(notdir $(CPP_FILES:.cpp=.o)))
CXXFLAGS += -std=c++11 -O2 -I$(INCLUDE_DIR) -MMD -Wall

all: release

debug: CXXFLAGS += -DDEBUG -g
debug: $(DEBUG_PATH)

release: $(RELEASE_PATH)

$(DEBUG_PATH): $(DEBUG_OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^

$(RELEASE_PATH): $(RELEASE_OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^

$(DEBUG_OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(RELEASE_OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(DEBUG_OBJ_DIR)/*.o $(DEBUG_OBJ_DIR)/*.d
	rm -rf $(RELEASE_OBJ_DIR)/*.o $(RELEASE_OBJ_DIR)/*.d

dump:
	objdump -d $(DEBUG_OBJ_FILES)

-include $(DEBUG_OBJ_FILES:.o=.d)
-include $(RELEASE_OBJ_FILES:.o=.d)