class Solution(object):
    def insert(self, intervals, newInterval):
        """
        :type intervals: List[List[int]]
        :type newInterval: List[int]
        :rtype: List[List[int]]
        """
        if len(intervals) == 0:
            return [newInterval]

        # locate the insert index of the newInterval
        # then use merge-interval strategy to merge intervals.
        insert_idx = len(intervals)
        for i in range(len(intervals)):
            if intervals[i][0] >= newInterval[0]:
                insert_idx = i
                break

        if insert_idx == 0:
            result = [newInterval]
            i = 0
        else:
            result = [intervals[0]]
            i = 1

        while i < len(intervals):
            if i == insert_idx:
                if result[-1][1] >= newInterval[0]:
                    result[-1][1] = max(result[-1][1], newInterval[1])
                else:
                    result.append(newInterval)
                insert_idx = -1
            else:
                if result[-1][1] >= intervals[i][0]:
                    result[-1][1] = max(result[-1][1], intervals[i][1])
                else:
                    result.append(intervals[i])
                i += 1

        if insert_idx == len(intervals):
            if result[-1][1] >= newInterval[0]:
                result[-1][1] = max(result[-1][1], newInterval[1])
            else:
                result.append(newInterval)

        return result
