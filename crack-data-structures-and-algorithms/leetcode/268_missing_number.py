# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def missing_number(nums):
    """
    :type nums: List[int]
    :rtype: int
    """
    # x ^ y = 0, if x = y; otherwise x ^ y = 1
    # suppose:
    # x0, x1, ..., xn complete sequence
    # y0, y1, ..., yn_1 missing yn
    # x0 ^ y0 = 0 because they are equal; same for other equal-pairs.
    # because 2nd sequence has lack of yn, which equals to xn, thus
    # final result: xn ^ 0 = xn i.e the missing number.
    x = 0
    i = 0
    while i < len(nums) + 1:
        x ^= i
        i += 1
    y = 0
    for e in nums:
        y ^= e
    return x ^ y