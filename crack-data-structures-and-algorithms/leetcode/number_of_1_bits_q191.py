# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def hw(n):
    cnt = 0
    while n > 0:
        if (n & 0x1) != 0:
            cnt += 1
        n >>= 1
    return cnt

print(hw(3) == 2)
print(hw(11) == 3)
print(hw(-3))
