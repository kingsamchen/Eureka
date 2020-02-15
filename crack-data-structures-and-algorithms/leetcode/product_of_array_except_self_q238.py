# 核心思路
# 要求O(N)同时不能用除法，所以不能用积除以当前元素的做法
# 将积拆成两部分：1）当前元素左边所有元素的积 2）当前元素右边所有元素的积
# 这两部分相乘就是所有元素除了当前元素的积
class Solution(object):
    def productExceptSelf(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        left_prod = [1] * len(nums)
        for i in range(1, len(nums)):
            left_prod[i] = left_prod[i-1] * nums[i-1]

        right_prod = [1] * len(nums)
        for i in range(len(nums) - 2, -1, -1):
            right_prod[i] = right_prod[i+1] * nums[i+1]

        result = []
        for i in range(len(nums)):
            result.append(left_prod[i] * right_prod[i])

        return result


# Followup: Could you solve it with constant space complexity?
# (The output array does not count as extra space for the purpose of space complexity analysis.)

# 按照题目要求，只能用最终的结果数组
# 观察前面的答案可以发现，不管是left_prod还是right_prod，都是一个累计值，我们实际上只需要保存这个累计值
# 所以用reslut数组保存第一遍left_prod的结果
# 然后计算right_prod时，用一个单独的 rprod 来保存累计值即可，同时每轮更新rprod
class Solution(object):
    def productExceptSelf(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        result = [1] * len(nums)

        for i in range(1, len(nums)):
            result[i] = result[i-1] * nums[i-1]

        rprod = 1
        for i in range(len(nums)-2, -1, -1):
            result[i] *= rprod * nums[i+1]
            rprod *= nums[i+1]

        return result
