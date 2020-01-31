class Solution(object):
    def sortColors(self, nums):
        """
        :type nums: List[int]
        :rtype: None Do not return anything, modify nums in-place instead.
        """
        # l points to last 0
        # r points to first 2
        # thus [l-1, r-1] contains all 1
        l = -1
        r = len(nums)
        i = 0
        # scan stops when reach r
        # when a[i] == 0, we make a[l+1] <-> a[i], because l < l+1 <= i,
        # we know a[l+1] is either 0 or 1, thus index i is examined.
        # but when a[i] == 2, we make a[i] <-> a[r-1], and we don't know what the value
        # the a[r-l] is, thus we don't increment i after swap; instead, we examine it again.
        while i < r:
            if nums[i] == 0:
                l += 1
                nums[l], nums[i] = nums[i], nums[l]
                i += 1
            elif nums[i] == 2:
                r -= 1
                nums[r], nums[i] = nums[i], nums[r]
            else:
                i += 1
