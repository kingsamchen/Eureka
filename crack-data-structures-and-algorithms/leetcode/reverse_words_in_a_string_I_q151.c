char* find_first_not(char* s, char c) {
    char* p = s;
    while (*p && *p == c) {
        ++p;
    }
    return p;
}

char* find_first(char* s, char c) {
    char* p = s;
    while (*p && *p != c) {
        ++p;
    }
    return p;
}

// [begin, end)
void reverse(char* begin, char* end) {
    for (char* l = begin, *r = end - 1; l < r; ++l, --r) {
        char tmp = *l;
        *l = *r;
        *r = tmp;
    }
}

char* reverseWords(char* s) {
    char* p = s;
    char* dst = s;
    char* tok_begin = NULL;
    char* tok_end = NULL;

    while (*p != '\0') {
        tok_begin = find_first_not(p, ' ');
        tok_end = find_first(tok_begin, ' ');
        reverse(tok_begin, tok_end);

        // points to the next ch after the space.
        p = tok_end + 1;
        // len includes the space.
        int len = tok_end - tok_begin + 1;
        // If we have already reached the end, one step backward.
        if (*tok_end == '\0') {
            --p;
            --len;
        }

        memmove(dst, tok_begin, len);
        dst += len;
    }

    // dst always points to the next tok_begin
    *dst = '\0';
    // we may still have a trailling space.
    if (dst > s && *(dst - 1) == ' ') {
        *--dst = '\0';
    }

    reverse(s, dst);

    return s;
}
