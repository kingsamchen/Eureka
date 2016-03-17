/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TRIE_FOR_DICT_ALPHABET_H_
#define TRIE_FOR_DICT_ALPHABET_H_

class Alphabet {
public:
    Alphabet() = delete;

    ~Alphabet() = delete;

    static size_t GetCharacterIndex(char ch);

    static size_t npos;
};


#endif  // TRIE_FOR_DICT_ALPHABET_H_