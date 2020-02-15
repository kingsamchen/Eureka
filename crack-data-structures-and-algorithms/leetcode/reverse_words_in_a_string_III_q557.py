# 核心思路
# 题目保证了每个单词之间只会有一个空格分隔，所以只需要简单的
# 按照空格切分即可得到每个token
# 然后对每个token reverse之后再拼接在一起
# 内建功能丰富的语言对这题比较简单
class Solution(object):
    def reverseWords(self, s):
        """
        :type s: str
        :rtype: str
        """
        tokens = s.split()
        for i in range(len(tokens)):
            tokens[i] = tokens[i][::-1]
        return ' '.join(tokens)
