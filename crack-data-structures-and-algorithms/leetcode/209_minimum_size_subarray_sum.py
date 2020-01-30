class Solution(object):
    def minSubArrayLen(self, s, nums):
        """
        :type s: int
        :type nums: List[int]
        :rtype: int
        """
        l = r = 0
        cur_sum = 0
        Guard = len(nums) + 1
        min_len = Guard
        # ensure we have scanned all elements
        while r < len(nums):
            # expand right bound
            while r < len(nums) and cur_sum < s:
                cur_sum += nums[r]
                r += 1

            # shrink left bound
            while cur_sum >= s:
                min_len = min(min_len, r - l)
                cur_sum -= nums[l]
                l += 1

        return 0 if min_len == Guard else min_len
