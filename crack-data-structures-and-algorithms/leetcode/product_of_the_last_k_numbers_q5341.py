# 核心思路
# 使用cache来缓存之前的查询结果，避免TLE
class ProductOfNumbers(object):

    def __init__(self):
        self._seq = []
        self._cache = {}

    def add(self, num):
        """
        :type num: int
        :rtype: None
        """
        self._seq.append(num)
        self._cache.clear()



    def getProduct(self, k):
        """
        :type k: int
        :rtype: int
        """
        if k in self._cache:
            return self._cache[k]

        prod = 1
        for i in range(-1, -k - 1, -1):
            e = self._seq[i]
            if e == 0:
                prod = 0
                break
            prod *= e

        self._cache[k] = prod

        return prod