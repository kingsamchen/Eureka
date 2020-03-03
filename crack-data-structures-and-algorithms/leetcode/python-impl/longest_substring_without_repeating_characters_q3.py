# 核心思路
# 采用 sliding window，window 内为不包含重复字符的子串
# 子串长度就是窗口大小
# left 来维护窗口的起始边界
class Solution(object):
    def lengthOfLongestSubstring(self, s):
        """
        :type s: str
        :rtype: int
        """
        longest = 0 # global longest
        left = 0    # maintains the left boundary of the sliding window.
        table = {}  # character -> index mapping
        for i in range(len(s)):
            ch = s[i]
            # If haven't seen or outside the window
            if ch not in table or table[ch] < left:
                table[ch] = i
            else:
                left = table[ch] + 1
                table[ch] = i
            # update result
            longest = max(longest, i - left + 1)

        return longest
