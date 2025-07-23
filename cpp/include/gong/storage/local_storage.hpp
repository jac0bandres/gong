#pragma once

#include "gong/storage/storage_provider.hpp"
#include <filesystem>

namespace gong::storage {

class LocalStorage : public StorageProvider {
public:
    explicit LocalStorage(const std::string& base_path = "");
    ~LocalStorage() override = default;
    
    // StorageProvider interface
    Result<void> connect() override;
    Result<void> disconnect() override;
    Result<bool> exists(const std::string& path) override;
    
    Result<void> upload_file(const std::string& local_path, 
                           const std::string& remote_path,
                           ProgressCallback callback = nullptr) override;
    
    Result<void> download_file(const std::string& remote_path,
                             const std::string& local_path,
                             ProgressCallback callback = nullptr) override;
    
    Result<void> delete_file(const std::string& path) override;
    
    Result<std::vector<FileInfo>> list_directory(const std::string& path) override;
    Result<void> create_directory(const std::string& path) override;
    Result<void> delete_directory(const std::string& path, bool recursive = false) override;
    
    Result<FileInfo> get_file_info(const std::string& path) override;
    
    std::string get_provider_name() const override { return "local"; }
    bool is_connected() const override { return connected_; }
    
private:
    std::filesystem::path base_path_;
    bool connected_ = false;
    
    std::filesystem::path resolve_path(const std::string& path) const;
    Result<void> copy_file_with_progress(const std::filesystem::path& from,
                                       const std::filesystem::path& to,
                                       ProgressCallback callback);
};

} // namespace gong::storage