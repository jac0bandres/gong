#include "gong/storage/local_storage.hpp"
#include "gong/common/result.hpp"
#include "gong/common/types.hpp"
#include <chrono>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <fmt/format.h>
#include <string>
#include <spdlog/spdlog.h>
#include <fstream>
#include <vector>

namespace gong::storage {
    LocalStorage::LocalStorage(const std::string& base_path_)
        : base_path_(base_path_.empty() ? std::filesystem::current_path().string() : base_path_) {}

    Result<void> LocalStorage::connect() {
        try {
            if (!std::filesystem::exists(base_path_)) {
                return Result<void>(fmt::format("Failed to connect to local storage: {}", base_path_.string()));
            }

            connected_ = true;
            spdlog::info("Connected to local storage {}", base_path_.string());
            return Result<void>();
        } catch(const std::exception& e) {
            return Result<void>(fmt::format("Failed to connect to local storage: {}", e.what()));
        }
    }

    Result<void> LocalStorage::disconnect() {
        connected_ = false;
        spdlog::info("Disconnected from local storage");
        return Result<void>();
    }

    Result<bool> LocalStorage::exists(const std::string& path) {
        try {
            auto full_path = resolve_path(path);
            return Result<bool>(std::filesystem::exists(full_path));
        } catch (const std::exception& e) {
            return Result<bool>(fmt::format("Error checking if path exists: {}", e.what()));
        }
    }

    Result<void> LocalStorage::upload_file(
        const std::string& local_path,
        const std::string& remote_path,
        ProgressCallback callback
    ) {
        try {
            auto src = std::filesystem::path(local_path);
            auto dst = resolve_path(remote_path);

            if (!std::filesystem::exists(src)) {
                return Result<void>(fmt::format("Source file does not exist: {}", local_path));
            }

            return copy_file_with_progress(src, dst, callback);
        } catch (const std::exception& e) {
            return Result<void>(fmt::format("Upload failed: {}", e.what()));
        }
    }

    Result<void> LocalStorage::download_file(
        const std::string& remote_path,
        const std::string& local_path,
        ProgressCallback callback
    ) {
        try {
            auto src = resolve_path(remote_path);
            auto dst = std::filesystem::path(local_path);

            if (!std::filesystem::exists(src)) {
                return Result<void>(fmt::format("Remote file does not exists: {}", remote_path));
            }

            return copy_file_with_progress(src, dst, callback);
        } catch (const std::exception& e) {
            return Result<void>(fmt::format("Download failed; {}", e.what()));
        }
    }

    Result<FileInfo> LocalStorage::get_file_info(const std::string& path) {
        try {
            auto full_path = resolve_path(path);

            if (!std::filesystem::exists(full_path)) {
                return Result<FileInfo>(fmt::format("Path does not exist: {}", path));
            }

            auto status = std::filesystem::status(full_path);
            auto size = std::filesystem::is_regular_file(status) ?
                std::filesystem::file_size(full_path) : 0;
            auto ftime = std::filesystem::last_write_time(full_path);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - std::filesystem::file_time_type::clock::now() +
                std::chrono::system_clock::now());

            FileInfo info(path, size, sctp, std::filesystem::is_directory(status));
            return Result<FileInfo>(std::move(info));
        } catch (const std::exception& e) {
            return Result<FileInfo>(fmt::format("Failed to get file info: {}", e.what()));
        }
    }

    std::filesystem::path LocalStorage::resolve_path(const std::string& path) const {
        auto p = std::filesystem::path(path);
        if (p.is_absolute()) {
            return p;
        }
        return base_path_ / p;
    }

    Result<void> LocalStorage::copy_file_with_progress(
        const std::filesystem::path& from,
        const std::filesystem::path& to,
        ProgressCallback callback
    ) {
        try {
            const size_t buffer_size = 64 * 1024; // 64kb buffer
            std::ifstream src(from, std::ios::binary);
            std::ofstream dst(to, std::ios::binary);

            if (!src) {
                return Result<void>(fmt::format("Cannon open source file: {}", from.string()));
            }

            if (!dst) {
                return Result<void>(fmt::format("Cannon create destination file: {}", to.string()));
            }

            auto file_size = std::filesystem::file_size(from);
            std::uint64_t copied = 0;
            std::vector<char> buffer(buffer_size);

            while (src.read(buffer.data(), buffer_size) || src.gcount() > 0) {
                auto bytes_read = src.gcount();
                dst.write(buffer.data(), bytes_read);

                copied += bytes_read;

                if (callback) {
                    callback(copied, file_size);
                }
            }

                spdlog::debug("Copied {} bytes from {} to {}", copied, from.string(), to.string());
                return Result<void>();
        } catch (const std::exception& e) {
            return Result<void>(fmt::format("Copy operation failed: {}", e.what()));
        }
    }

    Result<void> LocalStorage::create_directory(const std::string& path) {
        auto full_path = resolve_path(path);
        std::error_code ec;

        if (std::filesystem::create_directory(full_path, ec)) {
            return Result<void>();
        }
        
        return Result<void>(fmt::format("Couldn't create directory: {}", ec.message()));
    }
    //Result<void> LocalStorage::delete_directory()
    //Result<void> LocalStorage::list_directory()
}