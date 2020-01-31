# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Like Q1 but the array is sorted in ascending order, and
# returned index1 must be less than index2 and they are not zero-based

def two_sum(nums, target):
    """
    :type nums: List[int]
    :type target: int
    :rtype: List[int]
    """
    # Because the array is sorted, thus
    # If there are i, j s.t a[i] + a[j] = K then
    # a[i-1] + a[j] or a[i] + a[j-1] < k and a[i] + a[j+1] or a[i+1] + a[j] > k
    l = 0
    r = len(nums) - 1
    while l < r:
        sum = nums[l] + nums[r]
        if sum == target:
            return [l+1, r+1]
        if sum < target:
            l += 1
        else:
            r -= 1
    return []

nums = [2, 5, 7, 11]
target = 9
print(two_sum(nums, target))
