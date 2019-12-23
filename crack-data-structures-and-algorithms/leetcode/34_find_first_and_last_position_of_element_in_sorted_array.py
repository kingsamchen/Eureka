# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def search_range(nums, target):
    """
    :type nums: List[int]
    :type target: int
    :rtype: List[int]
    """
    lb = lower_bound(nums, target)
    if lb == len(nums) or nums[lb] != target:
        # No element matched, simply prune.
        return [-1, -1]

    rb = upper_bound(nums, target)
    if rb - 1 >= 0 and nums[rb-1] != target:
        return [-1, -1]
    else:
        rb -= 1

    return [lb, rb]

def lower_bound(nums, target):
    count = len(nums)
    first = 0
    while count > 0:
        half = count // 2
        mid = first + half
        if nums[mid] < target:
            first = mid + 1
            count -= half + 1
        else:
            count = half
    return first

def upper_bound(nums, target):
    count = len(nums)
    first = 0
    while count > 0:
        half = count // 2
        mid = first + half
        if not (target < nums[mid]):
            first = mid + 1
            count -= half + 1
        else:
            count = half
    return first

nums = [5,7,7,8,8,10]
target = 8
print(search_range(nums, target) == [3, 4])

nums = [5,7,7,8,8,10]
target = 6
print(search_range(nums, target) == [-1, -1])

nums = [2,4,6,8,10]
target = 6
print(search_range(nums, target) == [2, 2])

nums = [1]
target = 1
print(search_range(nums, target) == [0, 0])

nums = []
target = 1
print(search_range(nums, target) == [-1, -1])