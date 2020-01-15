class Solution(object):
    def merge(self, intervals):
        """
        :type intervals: List[List[int]]
        :rtype: List[List[int]]
        """
        if len(intervals) <= 1:
            return intervals

        # Ensure for any two intervals indexed at i and j, xi <= xj if i < j
        intervals.sort()

        results = [intervals[0]]
        for i in range(1, len(intervals)):
            cur = intervals[i]
            prev = results[-1]
            if cur[0] <= prev[1]:
                # Update only when new interval is larger.
                if cur[1] > prev[1]:
                    prev[1] = cur[1]
            else:
                results.append(cur)

        return results
