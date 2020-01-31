class Solution(object):
    def generateMatrix(self, n):
        """
        :type n: int
        :rtype: List[List[int]]
        """
        matrix = [[0] * n for _ in range(n)]

        col_cnt = n
        row_cnt = n - 1

        rop = 1
        cop = 1

        row, col = 0, 0

        value = 1

        while True:
            for _ in range(col_cnt):
                matrix[row][col] = value
                value += 1
                col += cop

            col_cnt -= 1
            col -= cop
            row += rop
            cop = -cop

            if row_cnt == 0:
                break

            for _ in range(row_cnt):
                matrix[row][col] = value
                value += 1
                row += rop

            row_cnt -= 1
            row -= rop
            col += cop
            rop = -rop

            if col_cnt == 0:
                break

        return matrix
