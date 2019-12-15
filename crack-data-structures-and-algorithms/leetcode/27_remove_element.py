# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Given nums = [0,1,2,2,3,0,4,2], val = 2
# Return length = 5

def remove_element_a(nums, val):
    j = i = 0
    # Loop invariant:
    # [a0...aj) maintains the qualified part.
    while i < len(nums):
        if nums[i] != val:
            nums[j] = nums[i]
            j += 1
        i += 1
    return j

def remove_element_b(nums, val):
    i = 0
    j = len(nums) - 1
    # Loop invariant:
    # [a0...ai) maintains the qualified part
    # scan from both directions, like pivot procedure in quicksort
    while i <= j:
        if nums[i] != val:
            i += 1
            continue
        if nums[j] == val:
            j -= 1
            continue
        nums[i], nums[j] = nums[j], nums[i]
    return i

#nums = [0, 1, 2, 2, 3, 0, 4, 2]
#nums = [3]
nums = [2]
val = 2
new_len = remove_element_b(nums, val)
print(new_len)
print(nums[0:new_len])
