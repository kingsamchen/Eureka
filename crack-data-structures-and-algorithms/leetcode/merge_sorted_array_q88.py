# -*- coding: utf-8 -*-
# 0xCCCCCCCC

# Given two sorted integer arrays nums1 and nums2, merge nums2 into nums1
# as one sorted array.
# The number of elements initialized in nums1 and nums2 are m and n respectively.
# You may assume that nums1 has enough space (size that is greater or equal to m + n)
# to hold additional elements from nums2.

def merge(nums1, m, nums2, n):
    """
    :type nums1: List[int]
    :type m: int
    :type nums2: List[int]
    :type n: int
    :rtype: None Do not return anything, modify nums1 in-place instead.
    """
    # Because we need to write new data in-place into nums1, thus
    # we need to handle two array from tail to head
    i = m - 1
    j = n - 1
    pos = m + n - 1
    while i >= 0 and j >= 0:
        if nums1[i] > nums2[j]:
            nums1[pos] = nums1[i]
            i -= 1
        else:
            nums1[pos] = nums2[j]
            j -= 1
        pos -= 1

    while j >= 0:
        nums1[pos] = nums2[j]
        j -= 1
        pos -= 1

# nums1, m = [1,2,3,0,0,0], 3
# nums2, n = [2,5,6], 3
nums1, m = [1,2,3,0,0], 3
nums2, n = [2, 6], 2
merge(nums1, m, nums2, n)
print(nums1)
