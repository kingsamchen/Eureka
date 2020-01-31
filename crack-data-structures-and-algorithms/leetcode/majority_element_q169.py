class Solution(object):
    def majorityElement(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        # since the majority element appears more than ceil(n/2) times
        # it must be at the mid (or right-mid if the array's length is even) if sorted.
        nums.sort()
        mid = (0 + len(nums)) // 2
        return nums[mid]
