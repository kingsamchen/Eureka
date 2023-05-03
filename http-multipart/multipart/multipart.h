//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef MULTIPART_MULTIPART_H_
#define MULTIPART_MULTIPART_H_

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

namespace http {

enum class read_status {
    ready,
    done,
    malformed
};

struct content_header {
    std::string_view name;
    std::string_view filename;
    std::string_view content_type;
};

// If `filename` is not empty, it is a file part.
struct content_part : private content_header {
    using content_header::content_type;
    using content_header::filename;
    using content_header::name;

    std::string_view data;
};

struct multipart_form {
    struct file {
        std::string filename;
        std::string data;
    };

    using value_table = std::unordered_map<std::string, std::string>;
    using file_table = std::unordered_map<std::string, file>;

    value_table values;
    file_table files;

    void add_part(const content_part& part) {
        if (part.filename.empty()) {
            values.emplace(part.name, part.data);
        } else {
            files.emplace(part.name, file{std::string{part.filename}, std::string{part.data}});
        }
    }
};

class multipart_reader {
public:
    // Throws
    //  - std::invalid_argument for empty body and invalid boundary.
    multipart_reader(std::string_view body, std::string_view boundary)
        : body_(body),
          boundary_(boundary) {
        if (body.empty()) {
            throw std::invalid_argument("empty body");
        }

        if (boundary.empty()) {
            throw std::invalid_argument("invalid boundary");
        }
    }

    // Returns true if a new part was read successfully.
    // Returns false if read has already completed or data is malformed.
    bool read_next_part(content_part& part);

    read_status status() const noexcept {
        return status_;
    }

private:
    // Returns true, if body prefix matches against `data`, and also removes
    // that part from body.
    // Returns false if not matched, and body is unchanged.
    bool consume_any(std::string_view data);

    // Returns true, if content header is consumed successfully.
    // Returns false, if malformed, and body is unchanged.
    bool consume_content_header(content_header& part_header);

    // Returns empty if malformed, and body keeps unchanged.
    std::string_view consume_data_part();

private:
    std::string_view body_;
    std::string_view boundary_;
    read_status status_{read_status::ready};
};

// Throws
//  - std::invalid_argument for empty or malformed body and invalid boundary.
multipart_form parse_multipart_form(std::string_view body, std::string_view boundary);

} // namespace http

#endif // MULTIPART_MULTIPART_H_
