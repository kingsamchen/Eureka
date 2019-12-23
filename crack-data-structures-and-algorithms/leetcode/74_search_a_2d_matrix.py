# -*- coding: utf-8 -*-
# 0xCCCCCCCC

def search_matrix(matrix, target):
    """
    :type matrix: List[List[int]]
    :type target: int
    :rtype: bool
    """
    # Locate row first.
    n = len(matrix)
    if n == 0:
        return False

    l = 0
    while n > 0:
        step = n // 2
        i = l + step
        if not matrix[i]:
            return False
        probe = matrix[i][0]
        if probe < target:
            l = i + 1
            n -= step + 1
        elif probe > target:
            n = step
        else:
            # Prune case.
            return True

    row = l - 1

    # Exclude the first element we have already examined.
    l, r = 1, len(matrix[row]) - 1
    while l <= r:
        m = (l + r) // 2
        e = matrix[row][m]
        if e == target:
            return True
        elif e > target:
            r = m - 1
        else:
            l = m + 1

    return False


matrix = [
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]
target = 3
print(search_matrix(matrix, target))

matrix = [[]]
target = 3
print(search_matrix(matrix, target))