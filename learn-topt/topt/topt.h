//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef TOPT_TOPT_H_
#define TOPT_TOPT_H_

#include <string>
#include <string_view>

namespace topt {

// Throws.
std::string generate_topt(std::string_view enc_secret);

} // namespace topt

#endif // TOPT_TOPT_H_
