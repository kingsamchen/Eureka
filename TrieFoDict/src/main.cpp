/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include <cassert>

#include "alphabet.h"
#include "basic_macros.h"
#include "trie.h"

void TestAlphabet()
{
    assert(Alphabet::length() == 27);

    assert(Alphabet::GetCharacterIndex('a') == 0);
    assert(Alphabet::GetCharacterIndex('-') == 26);
    assert(Alphabet::GetCharacterIndex(' ') == Alphabet::npos);
}

void TestTrieCtorDtor()
{
    Trie trie;
    UNREFED_VAR(trie);
}

void TestTrieInsert()
{
    Trie trie;
    trie.Insert("hello");
    //trie.Insert("go ahead");
}

void TestTrieContains()
{
    Trie trie;
    trie.Insert("hello");
    assert(trie.Contains("hello") == true);
    assert(trie.Contains("hell") == false);
    assert(trie.Contains("helloo") == false);
    assert(trie.Contains("he.llo") == false);
}

int main()
{
    TestTrieContains();
    return 0;
}
