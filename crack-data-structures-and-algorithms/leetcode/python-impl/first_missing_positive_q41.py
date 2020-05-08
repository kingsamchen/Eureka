class Solution(object):
    def firstMissingPositive(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        for i in range(len(nums)):
            while nums[i] > 0 and nums[i] <= len(nums) and nums[i] != nums[nums[i]-1]:
                t = nums[i] - 1
                nums[t], nums[i] = nums[i], nums[t]

        for i in range(len(nums)):
            if nums[i] != i + 1:
                return i + 1

        # existing values are all correct
        return len(nums) + 1
