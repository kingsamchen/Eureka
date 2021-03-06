/*
 @ 0xCCCCCCCC
*/

#include "alphabet.h"

#include <cstdlib>
#include <cstring>

namespace {

const char kAlphabet[] = "abcdefghijklmnopqrstuvwxyz-";

}

size_t Alphabet::npos = static_cast<size_t>(-1);

// static
size_t Alphabet::GetCharacterIndex(char ch)
{
    auto ptr = strchr(kAlphabet, ch);
    return ptr == nullptr ? npos : ptr - &kAlphabet[0];
}

// static
size_t Alphabet::length()
{
    // Don't forget to exclude the hidden null-terminator.
    return _countof(kAlphabet) - 1;
}