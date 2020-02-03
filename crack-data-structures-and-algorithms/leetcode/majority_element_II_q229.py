class Solution(object):
    def majorityElement(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        # Using Boyer-Moore Majority Vote Algorithm
        # Since we only use equality comparison, and cnt_a and cnt_b are initialized to 0
        # thus initializing a and b to 0 doesn't change result even if the nums contains negative numbers
        # or 0.
        a = b = 0
        cnt_a = cnt_b = 0
        for e in nums:
            if a == e:
                cnt_a += 1
            elif b == e:
                cnt_b += 1
            elif cnt_a == 0:
                a = e
                cnt_a = 1
            elif cnt_b == 0:
                b = e
                cnt_b = 1
            else:
                cnt_a -= 1
                cnt_b -= 1

        cnt_a = cnt_b = 0
        for e in nums:
            if e == a:
                cnt_a += 1
            elif e == b:
                cnt_b += 1

        result = []

        total = len(nums)

        if cnt_a > total // 3:
            result.append(a)

        if cnt_b > total // 3:
            result.append(b)

        return result
