class Solution(object):
    def search(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: bool
        """
        l, r = 0, len(nums)
        while l < r:
            m = (l + r) // 2
            if nums[m] == target:
                return True

            # Now that nums[m] != target, we analyze part by part.
            # left part is sorted
            if nums[l] < nums[m]:
                if nums[l] <= target < nums[m]:
                    r = m
                else:
                    l = m + 1
            # right part is sorted
            elif nums[l] > nums[m]:
                if nums[m] < target <= nums[r-1]:
                    l = m + 1
                else:
                    r = m
            # shrink
            else:
                l += 1

        return False