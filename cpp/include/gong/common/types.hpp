#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <cstdint>

namespace gong {
    struct FileInfo {
        std::string path;
        std::uint64_t size;
        std::chrono::system_clock::time_point modified_time;
        bool is_directory;
        std::string checksum;

        FileInfo() = default;
        FileInfo(
            std::string path, std::uint64_t size,
            std::chrono::system_clock::time_point modified_time,
            bool is_directory
        ) : path(std::move(path)), size(size), modified_time(modified_time), is_directory(is_directory) {}
    };

    // transger progress callback
    using ProgressCallback = std::function<void(std::uint64_t transferred, std::uint64_t total)>;

    // storage provider config
    struct StorageConfig {
        std::string provider_type;
        std::string endpoint;
        std::string access_key;
        std::string secret_key;
        std::string region;
        std::string bucket_name;

        // addition provider-specific options
        std::unordered_map<std::string, std::string> options;
    };
}