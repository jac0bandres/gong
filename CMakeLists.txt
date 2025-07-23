# In gong/CMakeLists.txt

cmake_minimum_required(VERSION 3.18)
project(GongProject LANGUAGES CXX)

# --- ADD THIS LINE ---
# This explicitly tells CMake to create the compile_commands.json file.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# ---------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(cpp)