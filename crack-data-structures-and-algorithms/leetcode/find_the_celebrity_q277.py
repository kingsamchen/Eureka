# The knows API is already defined for you.
# @param a, person a
# @param b, person b
# @return a boolean, whether a knows b
# def knows(a, b):

# 核心思路
# 保证O(n)时间复杂度，否则会TLE
# 第一步选取我们的候选celebrity，主要通过假定一个候选i，然后遍历j（i!=j)检查knows(i,j)返回值
# 如果返回True，则表明i认识j，则i一定不是候选者，将i替换为j，继续遍历；
# 如果返回False，则说明i有可能是候选者，继续遍历
# 第二轮是校验候选i是否是真的celebrity，所以需要从头遍历（跳过自己）
# 如果候选人i认识j或者j不认识i，则i不是celebrity；因为我们唯一的候选人出局了，所以表明当前条件下无celebrity
class Solution(object):
    def findCelebrity(self, n):
        """
        :type n: int
        :rtype: int
        """
        # our candidate celebrity cannot know anyone except itself.
        candidate = 0
        for i in range(1, n):
            if knows(candidate, i):
                candidate = i

        # our candidate may not be the true celebrity
        # lets verify it.
        for i in range(n):
            if candidate == i:
                continue
            if knows(candidate, i) or not knows(i, candidate):
                return -1

        # aha, it passes all tests.
        return candidate
