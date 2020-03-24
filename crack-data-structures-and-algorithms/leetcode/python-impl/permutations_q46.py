# 核心思路
# 首先排序保证数组为递增序（题目保证每个元素都不同）
# 然后不断调用next-permutation生成序列
class Solution(object):
    def permute(self, nums):
        """
        :type nums: List[int]
        :rtype: List[List[int]]
        """
        nums.sort()
        result = []
        while True:
            result.append(nums[:])
            if not next_perm(nums):
                break

        return result


def next_perm(nums):
    x, y = -1, -1
    for i in range(len(nums) - 1):
        if nums[i] < nums[i+1]:
            x = i

        if x != -1 and nums[x] < nums[i]:
            y = i

    # Have reached the last permutation.
    if x == -1:
        return False

    # Check the last element.
    if nums[-1] > nums[x]:
        y = len(nums) - 1

    nums[x], nums[y] = nums[y], nums[x]
    nums[x+1:] = reversed(nums[x+1:])

    return True