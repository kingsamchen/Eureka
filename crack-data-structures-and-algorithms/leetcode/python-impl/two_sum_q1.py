# 核心思路
# 使用hash-table作为辅助结构，保存 <number, index>
# 题目已经规定了只有一组解
class Solution(object):
    def twoSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[int]
        """
        table = {}
        for i in range(len(nums)):
            table[nums[i]] = i

        for i in range(len(nums)):
            delta = target - nums[i]
            if delta in table and table[delta] != i:
                return [i, table[delta]]

        return []
