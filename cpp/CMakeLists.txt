# In gong/cpp/CMakeLists.txt

# Make sure you have created these empty files:
# - gong/cpp/src/storage/local_storage.cpp
# - gong/cpp/src/storage/storage_provider.cpp

add_library(gong-core
    # List the source files here. This is crucial.
    src/storage/local_storage.cpp
    src/storage/storage_provider.cpp
)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(gong-core
    PUBLIC
        fmt::fmt
        spdlog::spdlog
)

target_include_directories(gong-core
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# ... rest of the file