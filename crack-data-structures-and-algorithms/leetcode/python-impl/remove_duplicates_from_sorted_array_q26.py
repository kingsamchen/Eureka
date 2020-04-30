# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Given sorted nums = [0,0,1,1,1,2,2,3,3,4]
# Return length = 5, and nums = [0,1,2,3,4]

def remove_duplicates(nums):
    cnt = len(nums)

    # Simply return if number of elements is less than 2
    if cnt < 2:
        return cnt

    # [a0...aj) matains qualified part, i.e. no duplicates within
    j = i = 1
    while i < cnt:
        if nums[i] != nums[j-1]:
            if i != j:
                nums[j] = nums[i]
            j += 1
        i += 1

    return j

#nums = [1,1,2]
nums = [0,0,1,1,1,2,2,3,3,4]
out = remove_duplicates(nums)
print('{}\n{}'.format(out, nums[:out]))
