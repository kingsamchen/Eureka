//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "multipart/multipart.h"

#include "esl/strings.h"

namespace strings = esl::strings;

namespace http {
namespace {

constexpr std::string_view newline = "\r\n";
constexpr std::string_view dashes = "--";
constexpr std::string_view hdr_content_disposition = "Content-Disposition:";
constexpr std::string_view hdr_content_type = "Content-Type:";
constexpr std::string_view param_name = "name=";
constexpr std::string_view param_filename = "filename=";

// Find `--boundary` in `str`.
std::size_t find_dashed_boundary(std::string_view str, std::string_view boundary) {
    static constexpr std::string_view delim_prefix = "\r\n--";

    // Content data is unlikely to contain boundary, so use boundary as locator.
    for (std::size_t pos = 0;;) {
        auto xpos = str.find(boundary, pos);
        if (xpos == std::string_view::npos || xpos < delim_prefix.size()) {
            return std::string_view::npos;
        }

        if (strings::starts_with(str.substr(xpos - delim_prefix.size()),
                                 delim_prefix)) {
            return xpos - dashes.size();
        }

        pos = xpos + boundary.size();
    }
}

// Content-Type: application/octet-stream
//  - any amount of linear whitespace after `:` is allowed.
bool read_header_content_type(std::string_view header_line, content_header& part_header) {
    auto type_value = strings::trim(header_line.substr(hdr_content_type.size()), " ");
    if (type_value.empty()) {
        return false;
    }

    part_header.content_type = type_value;

    return true;
}

// Content-Disposition: form-data; name="file"; filename="hello.txt"
//  - value must be `form-data`
//  - `name` param must also be present and must be immediately followed
//  - `filename` param is optional
//  - " for value of `name` and `filename` is recommended but not enforced
//  - any amount of linear whitespace after `:` and `;` is allowed
bool read_header_content_disposition(std::string_view header_line, content_header& part_header) {
    auto splitter = strings::split(header_line.substr(hdr_content_disposition.size()), ";");
    std::vector<std::string_view> params;
    constexpr std::size_t max_allowd_params = 3;
    params.reserve(max_allowd_params);
    for (auto token : splitter) {
        params.push_back(strings::trim(token, " "));
    }
    if (params.size() != 2 && params.size() != 3) {
        return false;
    }

    if (!strings::equals_ignore_ascii_case(params[0], "form-data") ||
        !strings::starts_with(params[1], param_name)) {
        return false;
    }

    if (params.size() == max_allowd_params) {
        if (!strings::starts_with(params[2], param_filename)) {
            return false;
        }

        params[2].remove_prefix(param_filename.size());
        part_header.filename = strings::trim(params[2], "\"");
    }

    params[1].remove_prefix(param_name.size());
    part_header.name = strings::trim(params[1], "\"");

    return true;
}

} // namespace

bool multipart_reader::read_next_part(content_part& part) {
    if (status() != read_status::ready) {
        return false;
    }

    status_ = read_status::malformed;

    if (!(consume_any(dashes) && consume_any(boundary_))) {
        return false;
    }

    // Boundary ends with --, end boundary reached.
    if (consume_any(dashes)) {
        if (body_ == newline) {
            status_ = read_status::done;
        }
        return false;
    }

    if (!consume_any(newline)) {
        return false;
    }

    // As per RFC, Content-Disposition header is mandatory, while Content-Type
    // is optional, but it seems the RFC doesn't say Content-Disposition must
    // come first.
    // We do not silently ignore presence of other content headers, instead,
    // we treat them as malformed to simplify our reader.
    content_header part_header;
    if (!consume_content_header(part_header) ||
        (!esl::strings::starts_with(body_, newline) && !consume_content_header(part_header)) ||
        part_header.name.empty()) {
        return false;
    }

    if (!consume_any(newline)) {
        return false;
    }

    auto part_data = consume_data_part();
    if (part_data.empty()) {
        return false;
    }

    part.name = part_header.name;
    part.content_type = part_header.content_type;
    part.filename = part_header.filename;
    part.data = part_data;

    status_ = read_status::ready;

    return true;
}

bool multipart_reader::consume_any(std::string_view data) {
    if (!esl::strings::starts_with(body_, data)) {
        return false;
    }

    body_.remove_prefix(data.size());

    return true;
}

bool multipart_reader::consume_content_header(content_header& part_header) {
    auto pos = body_.find(newline);
    if (pos == std::string_view::npos) {
        return false;
    }

    auto header_line = body_.substr(0, pos);
    if (strings::starts_with_ignore_ascii_case(header_line, hdr_content_disposition)) {
        if (!read_header_content_disposition(header_line, part_header)) {
            return false;
        }
    } else if (strings::starts_with_ignore_ascii_case(header_line, hdr_content_type)) {
        if (!read_header_content_type(header_line, part_header)) {
            return false;
        }
    } else {
        return false;
    }

    body_.remove_prefix(header_line.size() + newline.size());

    return true;
}

std::string_view multipart_reader::consume_data_part() {
    auto pos = find_dashed_boundary(body_, boundary_);
    if (pos == std::string_view::npos) {
        return {};
    }

    auto data = body_.substr(0, pos);
    data.remove_suffix(newline.size());

    body_.remove_prefix(pos);

    return data;
}

multipart_form parse_multipart_form(std::string_view body, std::string_view boundary) {
    multipart_reader reader(body, boundary);

    multipart_form form;
    for (content_part part; reader.read_next_part(part);) {
        form.add_part(part);
    }

    if (reader.status() != read_status::done) {
        throw std::invalid_argument("malformed request body");
    }

    return form;
}

} // namespace http
