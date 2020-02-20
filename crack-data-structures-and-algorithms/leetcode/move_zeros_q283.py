# 核心思路
# 类似std::remove()，如果当前元素不是0并且idx也不是target，说明需要腾挪
# [0, target)维护非零序列
# 将当前元素写道target指的位置，并前移target
# 因为当前元素需要腾挪说明现在这个位置的元素值已经无关紧要了，直接置零可以省掉后面清零操作
class Solution(object):
    def moveZeroes(self, nums):
        """
        :type nums: List[int]
        :rtype: None Do not return anything, modify nums in-place instead.
        """
        target = 0
        for i in range(len(nums)):
            if nums[i] != 0:
                if target != i:
                    nums[target] = nums[i]
                    nums[i] = 0
                target += 1
