class Solution(object):
    def subsetsWithDup(self, nums):
        """
        :type nums: List[int]
        :rtype: List[List[int]]
        """
        nums.sort()
        result = []
        do_subsets_with_dup(nums, 0, [], result)
        return result


def do_subsets_with_dup(nums, i, bk, result):
    if i == len(nums):
        result.append(bk[:])
        return

    nbk = bk[:]
    nbk.append(nums[i])
    # If current element is same as last element in bookkeeping, then
    # the current element must be included; otherwise we duplicate one entry.
    if len(bk) > 0 and bk[-1] == nums[i]:
        do_subsets_with_dup(nums, i + 1, nbk, result)
    else:
        do_subsets_with_dup(nums, i + 1, bk, result)
        do_subsets_with_dup(nums, i + 1, nbk, result)
