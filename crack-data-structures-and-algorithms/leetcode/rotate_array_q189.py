class Solution(object):
    def rotate(self, nums, k):
        """
        :type nums: List[int]
        :type k: int
        :rtype: None Do not return anything, modify nums in-place instead.
        """
        # typical one-dimensional vector rotation.
        # easier to understand than std::rotate from C++ STL
        pos = len(nums) - k % len(nums)
        reverse(nums, 0, pos)
        reverse(nums, pos, len(nums))
        reverse(nums, 0, len(nums))


def reverse(nums, first, last):
    l = first
    r = last - 1
    while l < r:
        nums[l], nums[r] = nums[r], nums[l]
        l += 1
        r -= 1
