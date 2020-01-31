class Solution(object):
    def trap(self, height):
        """
        :type height: List[int]
        :rtype: int
        """
        l, r = 0, len(height) - 1
        ub = 0
        vol = 0
        while l < r:
            lower = 0
            if height[l] < height[r]:
                lower = height[l]
                l += 1
            else:
                lower = height[r]
                r -= 1

            # update ub if lower is new ub.
            ub = max(ub, lower)
            # detal of current height and up is volum of water can trap vertically.
            vol += ub - lower

        return vol
