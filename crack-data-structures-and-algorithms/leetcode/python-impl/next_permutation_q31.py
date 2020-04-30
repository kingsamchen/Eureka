class Solution(object):
    def nextPermutation(self, nums):
        """
        :type nums: List[int]
        :rtype: None Do not return anything, modify nums in-place instead.
        """
        x, y = -1, -1
        # 1) find largest index x, s.t nums[x] < numx[x+1]
        # 2) find largest index y, s.t nums[x] < nums[y]; and because loop here terminates
        # before the last element, we need to check the last element for y particularly.
        for i in range(len(nums) - 1):
            if nums[i] < nums[i+1]:
                x = i

            if x != -1 and nums[i] > nums[x]:
                y = i

        if x == -1:
            nums.reverse()
            return

        # Because for loop only checks [0, |nums|-2], which skips the last element
        # we need to amend it manually.
        if nums[-1] > nums[x]:
            y = len(nums) - 1

        nums[x], nums[y] = nums[y], nums[x]
        reverse_part(nums, x+1, len(nums)-1)


def reverse_part(nums, l, r):
    while l < r:
        nums[l], nums[r] = nums[r], nums[l]
        l += 1
        r -= 1
