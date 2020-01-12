class Solution(object):
    def combinationSum2(self, candidates, target):
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



def combi_sum(values, idx, cur_sum, combi, results):
    if cur_sum == 0:
        results.append(combi[:])
        return

    # for array [a1, a2, a3, ..., an] we try each element from left to right
    # and in every try, saying we choose ai in ith trial, we examine possible combinations
    # in [ai+1, ..., an]
    for i in range(idx, len(values)):
        # i != idx indicates we are in the same round and check next element.
        if i != idx and values[i] == values[i-1]:
            continue

        next_sum = cur_sum - values[i]
        if next_sum < 0:
            return

        combi.append(values[i])
        combi_sum(values, i + 1, next_sum, combi, results)
        combi.pop()
