class Solution(object):
    def myAtoi(self, str):
        """
        :type str: str
        :rtype: int
        """
        sign = False
        value = 0

        # Skip leading spaces
        pos = 0
        while pos < len(str) and str[pos] == ' ':
            pos += 1

        # Handle sign
        if pos < len(str):
            if str[pos] == '+':
                pos += 1
            elif str[pos] == '-':
                pos += 1
                sign = True

        digits = '0123456789'
        while pos < len(str):
            ch = str[pos]
            if ch not in digits:
                break
            value = value * 10 + int(ch)
            pos += 1

        INT_MIN = -(1 << 31)
        INT_MAX = (1 << 31) - 1

        if sign:
            value = max(INT_MIN, -value)
        else:
            value = min(INT_MAX, value)

        return value
