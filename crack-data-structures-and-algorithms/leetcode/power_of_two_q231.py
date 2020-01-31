# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def is_power_of_two(n):
    """
    :type n: int
    :rtype: bool
    """
    # 00..010000 minus 1
    # 00..001111
    return False if n <= 0 else (n & (n - 1)) == 0
