class Solution(object):
    def combinationSum3(self, k, n):
        """
        :type k: int
        :type n: int
        :rtype: List[List[int]]
        """
        out = []
        results = []
        combi_sum(1, k, n, out, results)
        return results

def combi_sum(start, k, n, combi, results):
    # we have only choice now, save some time.
    if k == 1:
        if start <= n <= 9:
            combi.append(n)
            results.append(combi[:])
            combi.pop()
        return

    # check range [1, 9]
    for i in range(start, 10):
        combi.append(i)
        combi_sum(i + 1, k - 1, n - i, combi, results)
        combi.pop()
