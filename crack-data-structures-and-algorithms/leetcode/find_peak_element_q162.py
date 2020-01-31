# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def find_peak_element(nums):
    """
    :type nums: List[int]
    :rtype: int
    """
    # Turn to find local maximum within the array.
    l, r = 0, len(nums) - 1
    while l < r:
        m = (l + r) // 2
        if nums[m] < nums[m+1]:
            # Becase we want local maximum, m is excluded.
            l = m + 1
        else:
            # Included because m might be the local maximum
            r = m
    # l == r => target index
    return l


# Another solution with more intuition: https://leetcode.com/submissions/detail/287986266/

nums = [1,2,3,1]
print(find_peak_element(nums) == 2)

nums = [1,2,1,3,5,6,4]
ret = find_peak_element(nums)
print(ret == 5 or ret == 1)