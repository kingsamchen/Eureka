class Solution(object):
    def reverseWords(self, s):
        """
        :type s: List[str]
        :rtype: None Do not return anything, modify s in-place instead.
        """
        dst = 0
        tok_begin = tok_end = 0

        i = 0
        while i < len(s):
            tok_begin = find_first_not_space(s, i)
            tok_end = find_first_space(s, tok_begin)
            reverse(s, tok_begin, tok_end)

            i = tok_end + 1
            cnt = tok_end - tok_begin + 1
            if tok_end == len(s):
                i -= 1
                cnt -= 1

            for j in range(cnt):
                s[dst+j] = s[tok_begin+j]

            dst += cnt

        if dst > len(s) and s[dst - 1] == ' ':
            dst -= 1

        cnt = len(s) - dst - 1
        for _ in range(cnt):
            s.pop()

        s.reverse()


# [begin, end)
def reverse(s, begin, end):
    s[begin:end] = s[begin:end][::-1]


# Returns len(s) when not found
def find_first_space(s, pos):
    ch = ' '
    for i in range(pos, len(s)):
        if s[i] == ch:
            return i

    return len(s)


# Returns len(s) when not found
def find_first_not_space(s, pos):
    ch = ' '
    for i in range(pos, len(s)):
        if s[i] != ch:
            return i

    return len(s)
