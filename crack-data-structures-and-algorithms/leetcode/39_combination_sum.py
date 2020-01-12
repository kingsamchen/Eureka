class Solution(object):
    def combinationSum(self, candidates, target):
        """
        :type candidates: List[int]
        :type target: int
        :rtype: List[List[int]]
        """
        candidates.sort()
        out = []
        results = []
        combi_sum(candidates, 0, target, out, results)
        return results


# values must be sorted in order to prune.
# DFS strategy: always try values[idx] if possible.
def combi_sum(values, idx, cur_sum, combi, results):
    # one solution
    if cur_sum == 0:
        # deep clone
        results.append(combi[:])
        return

    for i in range(idx, len(values)):
        next_sum = cur_sum - values[i]
        if next_sum < 0:
            return

        combi.append(values[i])
        combi_sum(values, i, next_sum, combi, results)
        combi.pop()
