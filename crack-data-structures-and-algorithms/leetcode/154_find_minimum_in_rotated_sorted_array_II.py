# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Like Q153 but with possible duplicates.

def find_min(nums):
    """
    :type nums: List[int]
    :rtype: int
    """
    l, r = 0, len(nums) - 1
    while l < r and nums[l] >= nums[r]:
        m = (l + r) // 2
        if nums[m] > nums[r]:
            l = m + 1
        elif nums[m] < nums[r]:
            r = m
        else:
            # When nums[m] == nums[r], we have only few cases.
            # Try to prune cases as possible.
            if nums[m] < nums[l]:
                l += 1
                r = m
            # nums[l] = nums[m] = nums[r]
            # Rules out one of same elements, and properties of array are preserved.
            else:
                r -= 1
    return nums[l]

nums = [4,5,6,7,0,1,2]
print(find_min(nums))

nums = [3,4,5,1,2]
print(find_min(nums))

nums = [5,1,2]
print(find_min(nums))

nums = [5,2]
print(find_min(nums))

nums = [2,3,4,5,1]
print(find_min(nums))

nums = [1, 3, 5]
print(find_min(nums))

nums = [2,2,2,0,1]
print(find_min(nums))

nums = [3,3,1,3]
print(find_min(nums))

nums = [3,1,3,3,3]
print(find_min(nums))

nums = [4,4,4,4,4,4]
print(find_min(nums))