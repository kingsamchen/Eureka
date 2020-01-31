# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Given an array of integers, return indices of the two numbers such that
# they add up to a specific target.
# You may assume that each input would have exactly one solution, and
# you may not use the same element twice.

def two_sum(nums, target):
    """
    :type nums: List[int]
    :type target: int
    :rtype: List[int]
    """
    # use hable table for bookkeeping
    # we cannot exclude the case where two same numbers reside in the array
    # thus the value of the hash table is a list
    table = {}
    for i, num in enumerate(nums):
        if num not in table:
            table[num] = []
        table[num].append(i)

    for i, num in enumerate(nums):
        other = target - num
        if other in table:
            for idx in table[other]:
                if idx != i:
                    return [i, idx]
    return []

nums = [2, 7, 11, 5]
target = 9
print(two_sum(nums, target))
