class Solution(object):
    def uniquePaths(self, m, n):
        """
        :type m: int
        :type n: int
        :rtype: int
        """
        # Essentially: we must move s = m + n - 2 steps
        # use combination theory, choose C(s, n - 1) for how to move down

        total = m + n - 2
        base = min(m - 1, n - 1)

        a1 = 1
        for i in range(base):
            a1 *= total - i

        a2 = 1
        for i in range(1, base + 1):
            a2 *= i

        return a1 // a2
