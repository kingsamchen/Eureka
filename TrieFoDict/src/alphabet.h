
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TRIE_FOR_DICT_ALPHABET_H_
#define TRIE_FOR_DICT_ALPHABET_H_

class Alphabet {
public:
    Alphabet() = delete;

    ~Alphabet() = delete;

    static constexpr int GetCharacterIndex(char ch);
};

#endif  // TRIE_FOR_DICT_ALPHABET_H_