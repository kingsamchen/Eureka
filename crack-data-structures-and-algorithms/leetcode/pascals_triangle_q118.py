# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Given non-negative integer num_rows and generate pascal's triangle

def generate(num_rows):
    """
    :type numRows: int
    :rtype: List[List[int]]
    """
    # Special handling here.
    # Avoid branch condition for (1,1) in each inner loop
    if num_rows == 0: return []
    rows = [[1]]
    for i in range(2, num_rows+1):
        row = []
        for j in range(1, i+1):
            a = value_from_above(rows, i-1, j-1)
            b = value_from_above(rows, i-1, j)
            row.append(a+b)
        rows.append(row)
    return rows

def value_from_above(rows, x, y):
    if y == 0 or x < y:
        return 0
    return rows[x-1][y-1]

nr = 0
for r in generate(nr):
    print(r)
