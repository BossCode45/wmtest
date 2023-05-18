.PHONY: clean
CXX := g++
CXXFLAGS := -std=c++17  #-g # -fsanitize=address -fno-omit-frame-pointer
LINKFLAGS := -lX11
OBJS_DIR := .
OUT_DIR := .
SOURCE_DIR := .
EXEC := main
SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
SOURCE_HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
OBJS := $(subst $(SOURCE_DIR),$(OBJS_DIR), $(patsubst %.cpp,%.o,$(SOURCE_FILES)))

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LINKFLAGS) -o $(OUT_DIR)/$(EXEC)

$(OBJS_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#Files to be compiled
$(OBJS_DIR)/main.o: $(SOURCE_FILES) $(SOURCE_HEADERS)
$(OBJS_DIR)/commands.o: $(SOURCE_DIR)/commands.cpp $(SOURCE_DIR)/commands.h
$(OBJS_DIR)/config.o: $(SOURCE_DIR)/config.cpp $(SOURCE_DIR)/config.h
$(OBJS_DIR)/keybinds.o: $(SOURCE_DIR)/keybinds.cpp $(SOURCE_DIR)/keybinds.h $(SOURCE_DIR)/commands.h
clean:
	rm $(OBJS_DIR)/*.o 
	rm $(OUT_DIR)/$(EXEC)
