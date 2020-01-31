# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Given a non-negative index k where k ≤ 33, return the kth index row of the Pascal's triangle.
# Note that the row index starts from 0.

def get_row(row_index):
    """
    :type row_index: int
    :rtype: List[int]
    """
    bk_row = [1]
    for i in range(1, row_index+1):
        row = []
        for j in range(i+1):
            a = value_from_bk_row(bk_row, j-1)
            b = value_from_bk_row(bk_row, j)
            row.append(a+b)
        bk_row = row
    return bk_row

def value_from_bk_row(row, col):
    if col < 0 or col >= len(row):
        return 0
    return row[col]

nr = 4
print(get_row(nr))
