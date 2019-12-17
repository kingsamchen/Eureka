# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Given an array of digits such that the most significant digit is
# at the head of the list, and each element is a single digit
# Plus one to the integer
# Input: [1, 2, 3]
# Output: [1, 2, 4]

def plus_one(digits):
    i = len(digits) - 1
    cf = 1
    # If we have sth to add and we don't go out scope of the array.
    while i >= 0 and cf > 0:
        digits[i] += cf
        cf = 0
        if digits[i] == 10:
            digits[i] -= 10
            cf = 1
        i -= 1

    # Most significant digit carray.
    if cf > 0:
        digits.insert(0, 1)

    return digits

input = [4,3,2,1]
#input = [9,9,9]
#input = [1,2,3]
print(plus_one(input))