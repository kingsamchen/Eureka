/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "alphabet.h"

int main()
{
    std::string s = "hello world";
    std::cout << (Alphabet::GetCharacterIndex(' ') == Alphabet::npos) << std::endl;
    return 0;
}
