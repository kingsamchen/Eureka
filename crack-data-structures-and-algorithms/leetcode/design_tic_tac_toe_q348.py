# 核心思路
# 用两个一维数组来记录某一行或者某一列的内容情况
# p1在i,j落子，则row[i] += 1 && col[j] += 1；p2在i,j落子则row[i] -= 1 && col[j] -= 1
# 两个变量分别记录两个对角线的情况
class TicTacToe(object):

    def __init__(self, n):
        """
        Initialize your data structure here.
        :type n: int
        """
        self._row = [0] * n
        self._col = [0] * n
        self._dia = 0
        self._rdia = 0

    def move(self, row, col, player):
        """
        Player {player} makes a move at ({row}, {col}).
        @param row The row of the board.
        @param col The column of the board.
        @param player The player, can be either 1 or 2.
        @return The current winning condition, can be either:
                0: No one wins.
                1: Player 1 wins.
                2: Player 2 wins.
        :type row: int
        :type col: int
        :type player: int
        :rtype: int
        """
        mark = 1 if player == 1 else -1

        self._row[row] += mark
        self._col[col] += mark

        if row == col:
            self._dia += mark

        if row + col == len(self._row) - 1:
            self._rdia += mark

        n = len(self._row)
        if abs(self._row[row]) == n or abs(self._col[col]) == n or \
           abs(self._dia) == n or abs(self._rdia) == n:
           return 1 if mark == 1 else 2

        return 0


# Followup： O(n) 解法

# 用两个一维数组row[]和col[]来记录每一行或者每一列的落子情况
# player1在i,j落子的话，row[i]++ && col[j]++；而player2在i,j落子的话，row[i]-- && col[j]--
# 两个对角线用两个单独的变量即可，落子操作类似
# 只要绝对值等于n，则说明有一方已经下满了
def move(self, row, col, player):
    mark = 'X' if player == 1 else 'O'
    self._board[row][col] = mark

    done = True
    for x in self._board[row]:
        if x != mark:
            done = False
            break

    if done:
        return 1 if mark == 'X' else 2

    n = len(self._board)
    done = True
    for i in range(n):
        if self._board[i][col] != mark:
            done = False
            break

    if done:
        return 1 if mark == 'X' else 2

    if row == col:
        done = True
        for i in range(n):
            if self._board[i][i] != mark:
                done = False
                break

    if done:
        return 1 if mark == 'X' else 2

    if row + col == n - 1:
        done = True
        for i in range(n):
            if self._board[i][n-1-i] != mark:
                done = False
                break

    if done:
        return 1 if mark == 'X' else 2

    return 0
