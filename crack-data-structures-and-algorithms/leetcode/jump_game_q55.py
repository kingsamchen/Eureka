class Solution(object):
    def canJump(self, nums):
        """
        :type nums: List[int]
        :rtype: bool
        """
        if len(nums) == 0:
            return True

        # why greedy is enough?
        # If we can jump >= len(nums) steps in the first place, we can surely
        # jump to the last index.
        # Therefore, the essential of the problem is: when we scan to the last index
        # can steps >= 0.
        steps = nums[0]
        for i in range(1, len(nums)):
            steps -= 1
            if steps < 0:
                return False

            if nums[i] > steps:
                steps = nums[i]

        return True
