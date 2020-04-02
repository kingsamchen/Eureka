# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def is_palindrome(x):
    """
    :type x: int
    :rtype: bool
    """
    if x < 0:
        return False

    # if x == 0, then y == 0 and we will never go in the loop.
    y = 0
    n = x
    while n > 0:
        y = y * 10 + n % 10
        n //= 10

    return x == y

print(is_palindrome(-121))
print(is_palindrome(0))
print(is_palindrome(1))
print(is_palindrome(121))
print(is_palindrome(123))
