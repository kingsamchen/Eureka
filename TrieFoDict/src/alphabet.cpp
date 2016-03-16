
#include "alphabet.h"

#include <cstdlib>


constexpr const char kAlphabet[] = "abcdefghijklmnopqrstuvwxyz";

constexpr int GetCharacterIndexInternal(char ch, int pos)
{
    return pos >= _countof(kAlphabet) ? -1 :
        kAlphabet[pos] == ch ? pos : GetCharacterIndexInternal(ch, pos + 1);
}

// static
constexpr int Alphabet::GetCharacterIndex(char ch)
{
    return GetCharacterIndexInternal(ch, 0);
}