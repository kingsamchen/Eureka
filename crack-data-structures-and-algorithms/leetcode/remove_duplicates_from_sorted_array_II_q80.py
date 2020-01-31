# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Given a sorted array nums, remove the duplicates in-place such that
# duplicates appeared at most twice and return the new length.

# Aside: If we allow at most three duplicates?
def remove_duplicates_appeared_at_most_twice(nums):
    cnt = len(nums)
    # No need to handle this case, since even [2, 2] is permitted.
    if cnt < 3:
        return cnt

    j = i = 2
    while i < cnt:
        # [a0...aj) maintains the qualified part, i.e. at most two duplicates.
        # Because we can enlarge the qualified range when either
        # i)  a[i] != a[j-1], or
        # ii) a[i] == a[j-1] but a[i] != a[j-2]
        # we could simply eliminate comparison between a[i] & a[j-1].
        if nums[i] != nums[j-2]:
            nums[j] = nums[i]
            j += 1
        i += 1
    return j

#ary = [1,1,1,2,2,3]
ary = [0,0,1,1,1,1,2,3,3]
out = remove_duplicates_appeared_at_most_twice(ary)
print('{}\n{}'.format(out, ary[:out]))
