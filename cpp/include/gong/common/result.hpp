#pragma once

#include <stdexcept>
#include <utility>
#include <stdlib.h>
#include <variant>
#include <string>

namespace gong {
    template<typename T>
    class Result {
        public:
        // Success contructor
        Result(T&& value) : data_(std::move(value)) {}
        Result(const T& value) : data_(value) {}

        // Error constructor
        Result(std::string error) : data_(std::move(error)) {}

        // check if result contains value
        bool is_ok() const { return std::holds_alternative<T>(data_); }
        bool is_error() const { return std::holds_alternative<std::string>(data_); }

        const T& value() const {
            if (is_error()) {
                throw std::runtime_error("Attempted to get value from error result: " + error());
            }
            return std::get<T>(data_);
        }

        T& value() {
            if (is_error()) {
                throw std::runtime_error("Attempted to get value from error result: " + error());
                return std::get<T>(data_);
            }
        }

        // get error message
        const std::string& error() const {
            return std::get<std::string>(data_);
        }

        // convenience operators
        explicit operator bool() const { return is_ok(); }

        private:
        std::variant<T, std::string> data_;
};

    // void
    template<>
    class Result<void> {
        public:
        Result() : error_() {}
        Result(std::string error) : error_(std::move(error)) {}

        bool is_ok() const { return error_.empty(); }
        bool is_error() const { return !error_.empty(); }

        const std::string& error() const { return error_; }
        explicit operator bool() const { return is_ok (); }

        private:
        std::string error_;
    };
}