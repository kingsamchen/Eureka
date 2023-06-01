//
// Kingsley Chen
//

#pragma once

#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <string>
#include <string_view>
#include <system_error>

namespace llio {

void write_to_file(const std::string& path, std::string_view content, std::error_code& ec);

// Throws `std::filesystem::filesystem_error`
void write_to_file(const std::string& path, std::string_view content);

void read_from_file(const std::string& path, std::string& content, std::error_code& ec);

// Throws `std::filesystem::filesystem_error`
void read_from_file(const std::string& path, std::string& content);

} // namespace llio

#endif // FILE_IO_H_
