class Solution(object):
    def subsets(self, nums):
        """
        :type nums: List[int]
        :rtype: List[List[int]]
        """
        result = []
        do_subsets(nums, 0, [], result)
        return result

def do_subsets(nums, i, bk, result):
    if i == len(nums):
        result.append(bk)
        return

    nbk = bk[:]
    nbk.append(nums[i])

    # NOTE: if we want to optimize memory further, we can
    # save length of bk before we call branch 1
    # then restore bk to original length, and append current element, then run branch 2

    # the branch that doesn't contain the current element
    do_subsets(nums, i + 1, bk, result)
    # the branch that contains the current element
    do_subsets(nums, i + 1, nbk, result)
