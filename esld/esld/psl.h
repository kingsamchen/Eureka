//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef ESLD_PSL_H_
#define ESLD_PSL_H_

#include <string>
#include <string_view>

namespace psl {

inline const std::string rule_delim{"\0", 1};

std::string from_file(const std::string& path);

std::string from_data(std::string_view data);

std::string_view match_tld(std::string_view rule_data, const std::string& domain);

} // namespace psl

#endif // ESLD_PSL_H_
