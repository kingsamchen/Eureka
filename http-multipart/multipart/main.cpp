//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "fmt/format.h"

#include "multipart/multipart.h"

int main(int argc, const char* argv[]) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}

TEST_CASE("parse_multipart_form can parse correct multipart body") {
    const std::string field_name = "field1";
    const std::string field_vale = "value1";
    const std::string file_field_name = "myfile";
    const std::string filename = "hello.txt";
    const std::string file_data = "This is a test text in file hello.txt";
    const std::string boundary = "------------------------fbebd152e3df985c";
    auto body = fmt::format(
            "--------------------------fbebd152e3df985c\r\n"
            "Content-Disposition: form-data; name=\"{}\"; filename=\"{}\"\r\n"
            "Content-Type: application/octet-stream\r\n"
            "\r\n"
            "{}\r\n"
            "--------------------------fbebd152e3df985c\r\n"
            "Content-Disposition: form-data; name=\"{}\"\r\n"
            "\r\n"
            "{}\r\n"
            "--------------------------fbebd152e3df985c--\r\n",
            file_field_name, filename, file_data, field_name, field_vale);

    auto form = http::parse_multipart_form(body, boundary);
    REQUIRE_FALSE(form.values.empty());
    REQUIRE_FALSE(form.files.empty());

    CHECK_EQ(form.values.size(), 1);
    CHECK_EQ(form.values.count(field_name), 1);
    CHECK_EQ(form.values[field_name], field_vale);

    CHECK_EQ(form.files.size(), 1);
    CHECK_EQ(form.files.count(file_field_name), 1);
    CHECK_EQ(form.files[file_field_name].filename, filename);
    CHECK_EQ(form.files[file_field_name].data, file_data);
}

TEST_CASE("data contains dashes or part of boundary") {
    const std::string field_name = "field1";
    const std::string field_vale = "abc--defg";
    const std::string file_field_name = "myfile";
    const std::string filename = "hello.txt";
    const std::string file_data = "--fbebd152e3df985c";
    const std::string boundary = "------------------------fbebd152e3df985c";
    auto body = fmt::format(
            "--------------------------fbebd152e3df985c\r\n"
            "Content-Disposition: form-data; name=\"{}\"; filename=\"{}\"\r\n"
            "Content-Type: application/octet-stream\r\n"
            "\r\n"
            "{}\r\n"
            "--------------------------fbebd152e3df985c\r\n"
            "Content-Disposition: form-data; name=\"{}\"\r\n"
            "\r\n"
            "{}\r\n"
            "--------------------------fbebd152e3df985c--\r\n",
            file_field_name, filename, file_data, field_name, field_vale);

    auto form = http::parse_multipart_form(body, boundary);
    REQUIRE_FALSE(form.values.empty());
    REQUIRE_FALSE(form.files.empty());
}
