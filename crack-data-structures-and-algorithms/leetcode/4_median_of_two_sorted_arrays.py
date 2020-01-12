class Solution(object):
    def findMedianSortedArrays(self, nums1, nums2):
        """
        :type nums1: List[int]
        :type nums2: List[int]
        :rtype: float
        """
        len1 = len(nums1)
        len2 = len(nums2)
        m1 = (len1 + len2 + 1) // 2
        m2 = (len1 + len2 + 2) // 2
        val1 = getkth(nums1, 0, len1, nums2, 0, len2, m1)
        val2 = getkth(nums1, 0, len1, nums2, 0, len2, m2)
        return (val1 + val2) / 2.0


def getkth(a1, begin1, end1, a2, begin2, end2, k):
    len1 = end1 - begin1
    len2 = end2 - begin2

    # make sure |a1| >= |a2|
    if len1 < len2:
        return getkth(a2, begin2, end2, a1, begin1, end1, k)

    if len2 == 0:
        return a1[begin1+k-1]

    # base case
    # k will be eventually reduced to 1 had both arrays are not empty in this far.
    if k == 1:
        return min(a1[begin1], a2[begin2])

    mid1 = begin1 + min(len1, k // 2) - 1
    mid2 = begin2 + min(len2, k // 2) - 1
    if a1[mid1] < a2[mid2]:
        return getkth(a1, mid1 + 1, end1, a2, begin2, end2, k - (mid1 - begin1 + 1))
    else:
        return getkth(a1, begin1, end1, a2, mid2 + 1, end2, k - (mid2 - begin2 + 1))