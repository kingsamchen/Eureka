# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Given an array nums of n integers, are there elements a, b, c in nums such that
# a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.

def three_sum(nums):
    """
    :type nums: List[int]
    :rtype: List[List[int]]
    """
    # sort first; taken O(nlogn)
    # iterate from left to right to fix a, then turn this problem to two-sum
    # total taken O(n^2)
    # need to remove duplicate triplets
    nums.sort()
    results = []
    i = 0
    while i < len(nums) - 2:
        a = nums[i]
        if a > 0:
            break
        sum = -a
        bc = two_sum_pairs(nums[i+1:], sum)
        # Found match pairs
        if len(bc) != 0:
            for p in bc:
                results.append([a, p[0], p[1]])
            i += 1
            while i < len(nums) - 2 and nums[i-1] == nums[i]:
                i += 1
        else:
            i += 1

    return results


def two_sum_pairs(nums, val):
    l = 0
    r = len(nums) - 1
    pairs = []
    while l < r:
        sum = nums[l] + nums[r]
        if sum == val:
            pairs.append([nums[l], nums[r]])
            l += 1
            r -= 1
            # Since we just found a match, and if new l or new r equals to the old one
            # skip it becuase it must be duplicates.
            while l < r and nums[l-1] == nums[l]:
                l += 1
            while l < r and nums[r+1] == nums[r]:
                r -= 1
        else:
            if sum < val:
                l += 1
            else:
                r -= 1
    return pairs

nums = [-1, 0, 1, 2, -1, -4]
print(three_sum(nums))
nums = [-1, -1, 0, 1, 1]
print(three_sum(nums))
nums = [0,0,0,0]
print(three_sum(nums))
nums = [-2,0,1,1,2]
print(three_sum(nums))