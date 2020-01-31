# -*- coding: utf-8 -*-
# 0xCCCCCCCC


def search_matrix(matrix, target):
    """
    :type matrix: List[List[int]]
    :type target: int
    :rtype: bool
    """
    if len(matrix) == 0 or len(matrix[0]) == 0:
        return False

    if matrix[0][0] > target or matrix[-1][-1] < target:
        return False

    row = len(matrix) - 1
    col = 0
    while row >= 0 and col < len(matrix[0]):
        e = matrix[row][col]
        if e < target:
            col += 1
        elif e > target:
            row -= 1
        else:
            return True

    return False

m = [
  [1,   4,  7, 11, 15],
  [2,   5,  8, 12, 19],
  [3,   6,  9, 16, 22],
  [10, 13, 14, 17, 24],
  [18, 21, 23, 26, 30]
]
print(search_matrix(m, 5) == True)
print(search_matrix(m, 20) == False)
print(search_matrix(m, 0) == False)

print('---')

m = [
  [1,   4,  7, 11, 15],
  [2,   5,  8, 12, 19]
]
print(search_matrix(m, 5) == True)
print(search_matrix(m, 13) == False)
print(search_matrix(m, 2) == True)

print('---')

m = [[1,3,5]]
print(search_matrix(m, 5) == True)

m = [[1,4],[2,5]]
print(search_matrix(m, 2) == True)