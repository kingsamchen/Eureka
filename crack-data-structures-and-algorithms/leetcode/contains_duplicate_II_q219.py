class Solution(object):
    def containsNearbyDuplicate(self, nums, k):
        """
        :type nums: List[int]
        :type k: int
        :rtype: bool
        """
        memo = {}
        for i in range(len(nums)):
            val = nums[i]
            if val not in memo:
                memo[val] = i
            else:
                distance = i - memo[val]
                if distance <= k:
                    return True
                # old index is now useless, update to the new value.
                memo[val] = i

        return False
