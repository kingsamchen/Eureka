# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Find three integers s.t the sum is closest to the target.
# Return the sum of these three integers.

# Given array nums = [-1, 2, 1, -4], and target = 1.
# The sum that is closest to the target is 2.

def three_sum_closest(nums, target):
    """
    :type nums: List[int]
    :type target: int
    :rtype: int
    """
    # Like nomral 3-sum, but because we need to find the closest sum
    # thus we need bookkeeping and use offset to adjust scan direction.
    nums.sort()
    sum, dist = 0, 1 << 31
    i = 0
    while i < len(nums) - 2:
        l = i + 1
        r = len(nums) - 1
        while l < r:
            attempt = nums[i] + nums[l] + nums[r]
            offset = abs(attempt - target)
            # New closest found
            if offset < dist:
                dist = offset
                sum = attempt
            # If attempt < target, the sum is at the left of the target.
            # If attempt > target, the sum is at the right of the target.
            if attempt < target:
                l += 1
            elif attempt > target:
                r -= 1
            else:
                return sum
        i += 1
    return sum

nums = [-1, 2, 1, -4]
target = 1
print(three_sum_closest(nums, target))