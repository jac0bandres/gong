#include <gtest/gtest.h>
#include "gong/storage/local_storage.hpp"
#include <filesystem>

// Test fixture for creating a temporary directory for tests
class LocalStorageTest : public ::testing::Test {
    protected:
    void SetUp() override {
        // create a unique temp dir for each test
        temp_dir = std::filesystem::temp_directory_path() / "gong_test_run";
        std::filesystem::create_directory(temp_dir);
    }

    void TearDown() override {
        std::filesystem::remove_all(temp_dir);

    }

    std::filesystem::path temp_dir;
};

// test case for connect/disconnect
TEST_F(LocalStorageTest, Connection) {
    gong::storage::LocalStorage storage(temp_dir.string());

    EXPECT_FALSE(storage.is_connected());

    auto connect_result = storage.connect();
    ASSERT_TRUE(connect_result.is_ok()) << connect_result.error();
    EXPECT_TRUE(storage.is_connected());

    auto disconnect_result = storage.disconnect();
    ASSERT_TRUE(disconnect_result.is_ok()) << disconnect_result.error();
    EXPECT_FALSE(storage.is_connected());
}

// test case for creating a dir
TEST_F(LocalStorageTest, CreateDirectory) {
    gong::storage::LocalStorage storage(temp_dir.string());
    storage.connect();

    auto create_result = storage.create_directory("new_dir");
    ASSERT_TRUE(create_result.is_ok()) << create_result.error();

    auto exists_result = storage.exists("new_dir");
    ASSERT_TRUE(exists_result.is_ok()) << exists_result.error();
    EXPECT_TRUE(exists_result.value());
}