class Solution(object):
    def maxProduct(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        if len(nums) == 0:
            return 0

        # max_p[i] saves maximum product of subarray among nums[0...i], both inclusive
        # min_p[i] saves minimum product of subarray among nums[0...i], both inclusive
        # because multiplication of two negative integers produce positive integer, thus
        # max_p[i] is from max_p[i-1] * nums[i], or min_p[i-1] * nums[i], or simply nums[i] ( a new bookkeeping)
        # Also, because the final result may not include nums[n-1], we need result to keep global maximum
        max_p = [0] * len(nums)
        min_p = [0] * len(nums)
        result = max_p[0] = min_p[0] = nums[0]
        for i in range(1, len(nums)):
            max_p[i] = max(max_p[i-1] * nums[i], min_p[i-1] * nums[i], nums[i])
            min_p[i] = min(max_p[i-1] * nums[i], min_p[i-1] * nums[i], nums[i])
            result = max(max_p[i], result)

        return result


# With status space compression

class Solution2(object):
    def maxProduct(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        if len(nums) == 0:
            return 0

        max_p = nums[0]
        min_p = nums[0]
        result = nums[0]
        for i in range(1, len(nums)):
            old_max = max_p
            old_min = min_p
            max_p = max(old_max * nums[i], old_min * nums[i], nums[i])
            min_p = min(old_max * nums[i], old_min * nums[i], nums[i])
            result = max(max_p, result)

        return result
