// 核心思路
// 将数字反转得到 val，如果 val == x 则说明数字是回文数字
// 1) 负数因为有-号，所以肯定不满足，可以提前结束
// 2) INT_MAX 反转会溢出，因为这里不会出现负数，所以可以先将 x 转型为 unsigned
//    然后再做反转
class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0) {
            return false;
        }

        unsigned int val = 0;
        unsigned int num = x;
        while (num != 0) {
            val = val * 10 + num % 10;
            num /= 10;
        }

        return val == x;
    }
};
