CXX      = clang++
STRIP    = aarch64-linux-android-strip

# 2. Compiler and Linker Flags
# Corresponds to CMAKE_CXX_STANDARD 17
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fPIC

# Corresponds to target_include_directories
INCLUDES = -I. \
           -IImGui \
           -IImGui/backends \
           -IDobby \
           -IByNameModding

# Corresponds to target_link_libraries
# Note: ${CMAKE_CURRENT_SOURCE_DIR}/Dobby/libdobby.a is included directly
LDFLAGS  = -shared \
           Dobby/libdobby.a \
           -llog \
           -landroid \
           -lEGL \
           -lGLESv3

# 3. Target Output Name (SHARED library translates to lib[name].so)
TARGET   = libtest.so

# 4. Source Files (add_library)
SRCS     = main.cpp \
           ImGui/imgui.cpp \
           ImGui/imgui_draw.cpp \
           ImGui/imgui_tables.cpp \
           ImGui/imgui_widgets.cpp \
           ImGui/backends/imgui_impl_android.cpp \
           ImGui/backends/imgui_impl_opengl3.cpp \
           ByNameModding/fake_dlfcn.cpp \
           ByNameModding/Il2Cpp.cpp

# 5. Object Files (maps .cpp to .o inside a build directory to stay organized)
OBJ_DIR  = build
OBJS     = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# 6. Default Rule
.PHONY: all
all: $(TARGET)

# Rule to link the shared library
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS)
	@echo "Successfully built $(TARGET)"

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Cleanup Rule
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
