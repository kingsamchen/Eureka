class Solution(object):
    def exist(self, board, word):
        """
        :type board: List[List[str]]
        :type word: str
        :rtype: bool
        """
        if len(board) == 0 or len(board[0]) == 0:
            return False

        rcnt = len(board)
        ccnt = len(board[0])

        bk = [[0] * ccnt for _ in range(rcnt)]

        for r in range(rcnt):
            for c in range(ccnt):
                if board[r][c] == word[0]:
                    result = search(board, rcnt, ccnt, r, c, bk, 1, word)
                    if result:
                        return True
        return False



# semantics: board[r][c] is matched and now being the current start point.
# try to search against word[i].
# function marks bk[r][c] as 1 because the function is able to confirm or withdraw.
def search(board, rcnt, ccnt, r, c, bk, i, word):
    bk[r][c] = 1

    if i == len(word):
        return True

    target = word[i]

    if c + 1 < ccnt and board[r][c + 1] == target and bk[r][c + 1] == 0 and \
            search(board, rcnt, ccnt, r, c + 1, bk, i + 1, word):
        return True

    if r + 1 < rcnt and board[r + 1][c] == target and bk[r + 1][c] == 0 and \
            search(board, rcnt, ccnt, r + 1, c, bk, i + 1, word):
        return True

    if c - 1 >= 0 and board[r][c - 1] == target and bk[r][c - 1] == 0 and \
            search(board, rcnt, ccnt, r, c - 1, bk, i + 1, word):
        return True

    if r - 1 >= 0 and board[r - 1][c] == target and bk[r - 1][c] == 0 and \
            search(board, rcnt, ccnt, r - 1, c, bk, i + 1, word):
        return True

    bk[r][c] = 0

    return False
