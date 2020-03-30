# 核心思路
# 两个数组分别保存所有的开始时间和结束时间，排序
# 计算有多少个区间可能重合的次数，等于需要会议室的数量
class Solution(object):
    def minMeetingRooms(self, intervals):
        """
        :type intervals: List[List[int]]
        :rtype: int
        """
        if len(intervals) == 0:
            return 0

        stime = []
        etime = []
        for x in intervals:
            stime.append(x[0])
            etime.append(x[1])

        stime.sort()
        etime.sort()

        room_cnt = 0
        epos = 0
        for i in range(len(intervals)):
            if stime[i] < etime[epos]:
                room_cnt += 1
            else:
                # reuse the room, and update our endtime of room
                epos += 1

        return room_cnt


# 一个 suboptimal 但是很直接的解法
# rooms 保存已经开辟的会议室，如果一个会议室被复用，则延长结束时间
class Solution(object):
    def minMeetingRooms(self, intervals):
        """
        :type intervals: List[List[int]]
        :rtype: int
        """
        if len(intervals) == 0:
            return 0

        intervals.sort()
        rooms = [intervals[0][:]]
        for i in range(1, len(intervals)):
            found = False
            for j in range(len(rooms)):
                if intervals[i][0] >= rooms[j][1]:
                    # extend room occupation time
                    rooms[j][1] = intervals[i][1]
                    found = True
                    break
            # If no room can be reused, make a new room.
            if not found:
                rooms.append(intervals[i][:])

        return len(rooms)
