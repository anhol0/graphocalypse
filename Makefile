IMGUI_DIR = external/imgui
IMGUI_LIB = $(IMGUI_DIR)/libimgui.a

RLIMGUI_DIR = external/rlImGui
RLIMGUI_LIB = $(RLIMGUI_DIR)/librlimgui.a

RAYLIB_DIR = external/raylib/src
RAYLIB_LIB = $(RAYLIB_DIR)/libraylib.a

RAYLIB_SHADERS = external/raylib/examples/shaders

SOURCES = src/engine/main.cpp 
INCLUDES = -I$(IMGUI_DIR) -I$(RLIMGUI_DIR) -I$(RAYLIB_DIR) -I$(RAYLIB_SHADERS)
CFLAGS = -g -Wall
LDFLAGS = -lraylib\
				  -ldl \
					-lpthread -lGL -lm \
					-L$(IMGUI_DIR) -limgui \
					-L$(RLIMGUI_DIR) -lrlimgui \
					-L$(RAYLIB_DIR) -lraylib
BUILD_DIR = build
TARGET = $(BUILD_DIR)/graphocalypse
CXX = g++

all: clean $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCES) $(IMGUI_LIB) $(RLIMGUI_LIB) $(RAYLIB_LIB)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(SOURCES) $(INCLUDES) -o $(TARGET) $(LDFLAGS)

$(IMGUI_LIB): $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp \
              $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
	$(CXX) -c $^ -I$(IMGUI_DIR)
	ar rcs $@ imgui*.o
	rm imgui*.o  

$(RLIMGUI_LIB): $(RLIMGUI_DIR)/rlImGui.cpp
	g++ -c $< -I$(RLIMGUI_DIR) -I$(IMGUI_DIR) -I$(RAYLIB_DIR)
	ar rcs $@ rlImGui*.o
	rm rlImGui*.o

$(RAYLIB_LIB):
	@if [ ! -f $@ ]; then \
		echo "Building raylib..."; \
		cd $(RAYLIB_DIR) && $(MAKE) PLATFORM=PLATFORM_DESKTOP; \
	fi

clean:
	rm -rf $(BUILD_DIR)

clean-all:
	rm -rf $(BUILD_DIR) $(IMGUI_LIB) $(RLIMGUI_LIB) $(RAYLIB_LIB)