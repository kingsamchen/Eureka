class Solution(object):
    def setZeroes(self, matrix):
        """
        :type matrix: List[List[int]]
        :rtype: None Do not return anything, modify matrix in-place instead.
        """
        if len(matrix) == 0 or len(matrix[0]) == 0:
            return matrix

        rcnt = len(matrix)
        ccnt = len(matrix[0])

        horz, verz = False, False

        for i in range(ccnt):
            if matrix[0][i] == 0:
                horz = True
                break

        for i in range(rcnt):
            if matrix[i][0] == 0:
                verz = True
                break

        # exclude outmost row and column, and if one cell at (i,j) is 0
        # then mark (i, 0) and (0, j) as 0.
        # i.e. we use these two outmost row and column for bookkeeping.
        for i in range(1, rcnt):
            for j in range(1, ccnt):
                if matrix[i][j] == 0:
                    matrix[i][0] = 0
                    matrix[0][j] = 0

        for i in range(1, rcnt):
            for j in range(1, ccnt):
                if matrix[i][0] == 0 or matrix[0][j] == 0:
                    matrix[i][j] = 0

        # zero outmost row & col if necessary.

        if horz:
            for i in range(ccnt):
                matrix[0][i] = 0

        if verz:
            for i in range(rcnt):
                matrix[i][0] = 0
