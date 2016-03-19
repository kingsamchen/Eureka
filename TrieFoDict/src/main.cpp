/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include <cassert>

#include "alphabet.h"
#include "basic_macros.h"
#include "trie.h"

void AssertAlphabet()
{
    assert(Alphabet::length() == 27);

    assert(Alphabet::GetCharacterIndex('a') == 0);
    assert(Alphabet::GetCharacterIndex('-') == 26);
    assert(Alphabet::GetCharacterIndex(' ') == Alphabet::npos);
}

void AssertTrieCtorDtor()
{
    Trie trie;
    UNREFED_VAR(trie);
}

void AssertTrieInsert()
{
    Trie trie;
    trie.Insert("hello");
    //trie.Insert("go ahead");
}

int main()
{
    AssertTrieInsert();
    return 0;
}
