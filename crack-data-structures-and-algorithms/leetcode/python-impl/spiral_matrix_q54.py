class Solution(object):
    def spiralOrder(self, matrix):
        """
        :type matrix: List[List[int]]
        :rtype: List[int]
        """
        if len(matrix) == 0 or len(matrix[0]) == 0:
            return []

        xcnt = len(matrix[0])
        ycnt = len(matrix) - 1
        iop = 1
        jop = 1
        i, j = 0, 0
        results = []
        while True:
            for _ in range(xcnt):
                results.append(matrix[i][j])
                j += jop
            xcnt -= 1

            # Tweak i & j
            j -= jop
            i += iop

            # make op opposite
            jop = -jop

            if ycnt == 0:
                break

            for _ in range(ycnt):
                results.append(matrix[i][j])
                i += iop

            ycnt -= 1
            i -= iop
            j += jop
            iop = -iop

            if xcnt == 0:
                break

        return results