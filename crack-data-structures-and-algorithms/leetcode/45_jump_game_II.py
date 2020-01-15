class Solution(object):
    def jump(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        if len(nums) <= 1:
            return 0

        jump = 0
        steps = nums[0]
        cnt = len(nums)
        i = 0
        while True:
            if steps >= cnt - (i + 1):
                jump += 1
                break

            max_dist = 0
            move = 0
            for j in range(1, steps + 1):
                new_steps = nums[i+j]
                new_dist = new_steps + j
                if new_dist >= max_dist:
                    max_dist = new_dist
                    move = j
                    steps = new_steps

            i += move
            jump += 1

        return jump
