# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def find_min(nums):
    """
    :type nums: List[int]
    :rtype: int
    """
    l, r = 0, len(nums) - 1
    while nums[l] > nums[r]:
        m = (l + r) // 2
        if nums[m] > nums[r]:
            l = m + 1
        elif nums[m] < nums[r]:
            r = m
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