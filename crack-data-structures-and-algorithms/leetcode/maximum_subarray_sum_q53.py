class Solution(object):
    def maxSubArray(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        best_sum = -(1 << 32)
        cur_sum = 0
        for ele in nums:
            cur_sum += ele
            if cur_sum > best_sum:
                best_sum = cur_sum
            # discard this cumulation once it becomes negative, because
            # 1) if the next element is positive, better to start from this positive element.
            # 2) if the next element is negative, sum it only makes result smaller.
            if cur_sum < 0:
                cur_sum = 0
        return best_sum
