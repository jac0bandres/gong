#pragma once

#include "gong/common/result.hpp"
#include "gong/common/types.hpp"
#include <memory>
#include <vector>
#include <functional>

namespace gong::storage {
    class StorageProvider {
        public:
        virtual ~StorageProvider() = default;

        // basic ops
        virtual Result<void> connect() = 0;
        virtual Result<void> disconnect() = 0;
        virtual Result<bool> exists(const std::string& path) = 0;

        // file ops
        virtual Result<void> upload_file(
            const std::string& local_path,
            const std::string& remove_path,
            ProgressCallback callback = nullptr
        ) = 0;

        virtual Result<void> download_file(
            const std::string& remote_path,
            const std::string& local_path,
            ProgressCallback callback = nullptr
        ) = 0;

        //virtual Result<void> delete_file(const std::string& path) = 0;

        // directory ops
        //virtual Result<std::vector<FileInfo>> list_directory(const std::string& path) = 0;
        virtual Result<void> create_directory(const std::string& path) = 0;
        //virtual Result<void> delete_directory(const std::string& path, bool recursive = false) = 0;

        // metaddata ops
        virtual Result<FileInfo> get_file_info(const std::string& path) = 0;

        // provider info
        virtual std::string get_provider_name() const = 0;
        virtual bool is_connected() const = 0;
    };

    // factory function
    std::unique_ptr<StorageProvider> create_storage_provider(const StorageConfig& config);
}