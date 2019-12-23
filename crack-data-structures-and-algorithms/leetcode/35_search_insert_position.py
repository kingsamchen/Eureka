# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def search_insert(nums, target):
    """
    :type nums: List[int]
    :type target: int
    :rtype: int
    """
    # Simply lower_bound
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


ary, t = [1,3,5,6], 5
print(search_insert(ary, t) == 2)

ary, t = [1,3,5,6], 2
print(search_insert(ary, t) == 1)

ary, t = [1,3,5,6], 7
print(search_insert(ary, t) == 4)

ary, t = [1,3,5,6], 0
print(search_insert(ary, t) == 0)
