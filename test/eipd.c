

double evl(int *brd, int *pce_x, int pos_y, int col, int col_max, int rw_n)
{
    brd.reverse();
    float scr = 0;
    int ld_h = get_ld_h(pce_x, pos_y, rw_n);
    int rw_rm = get_rw_rm(brd, col_max, rw_n);
    int rw_t = get_rw_t(brd, col, rw_n);
    int col_t = get_col_t(brd, col, rw_n, col_max);
    int hl = gt_hl(brd, col, rw_n);
    int wl = gt_wl(brd, col, rw_n);
    double scr += ld_h * -4.500158825082766;
    scr += rw_rm * 3.4181268101392694;
    scr += rw_t * -3.2178882868487753;
    scr += col_t * -9.348695305445199;
    scr += hl * -7.899265427351652;
    scr += wl * -3.3855972247263626;
    return scr;
}

int get_ld_h(int *pce_x, int pce_x_size, int pos_y, int rw_n)
{
    int p_h = 0;
    int c = 0;
    for (int i; i< pce_x_size; i++) {
        if (pce_x[i])
            continue;
        c += 1;
        p_h += ((rw_n - 1) - (pos_y + i));
    }
    return p_h/c;
}

int get_rw_n(int *brd)
{
    return len(brd);
}



int get_rw_rm(int *brd, int brd_size, int col_max, int rw_n)
{
    int c = 0;
    for idx, row in enumerate(brd):
    for(int i=0; i< brd_size; i++)
        if (brd[i] == col_max) {
            c += 1;
            brd.pop(brd[i]);
            brd.insert(rw_n - 1, 0);
        } else if (brd[i] == 0) {
            break;
        }
    return c
}

int get_rw_t(int *brd, int col, int rw_n)
{
    int t = 0;
    int last_bit = 1;
    for (int i in range(rw_n):
        row = brd[i]
        if not row:
            t += 2
            last_bit = 1
            continue
        for j in range(col):
            bit = (row >> j) & 1
            if bit != last_bit:
                t += 1
                last_bit = bit
        if not bit:
            t += 1
        last_bit = 1
    return t
}

def get_col_t(brd: list, col: int, rw_n: int, col_max: int) -> int:
    """
    The total number of col transitions.
    A col transition occurs when an empty cell is adjacent to a filled cell
    on the same row and vice versa.
    :param brd, the ctx data
    :param col, col count of the board
    :param rw_n, the row count of board
    :param col_max, the max value of column
    :return the point transition count in col direction
    """
    t = 0
    xor = brd[0] ^ col_max
    for j in range(col):
        if (xor >> j) & 1:
            t += 1
    for i in range(1, rw_n):
        xor = brd[i] ^ brd[i - 1]
        for j in range(col):
            if (xor >> j) & 1:
                t += 1
    return t


def get_col_t1(brd: list, col: int, rw_n: int, col_max: int) -> int:
    t = 0
    last_bit = 1
    for i in range(col):
        for j in range(rw_n):
            row = brd[j]
            bit = (row >> i) & 1
            if bit != last_bit:
                t += 1
            last_bit = bit
        last_bit = 1
    return t


def gt_hl(brd: list, col: int, rw_n: int) -> int:
    """
    get hole count number
    :param brd the ctx data list
    :param col
    :param rw_n, the row count of board
    :return hole number
    """
    hl = 0
    rw_hl = 0b0000000000
    p_rw = brd[rw_n - 1]
    for i in range(rw_n - 2, -1, -1):
        c_rw = brd[i]
        rw_hl = ~c_rw & (p_rw | rw_hl)
        for j in range(col):
            hl += ((rw_hl >> j) & 1)
        p_rw = brd[i]
    return hl


def gt_wl(brd: list, col: int, rw_n: int) -> int:
    """
    A well is a sequence of empty cells above the top piece in a col such
    that the top cell in the sequence is surrounded (left and right) by occupied
    cells or a boundary of the board.

    :param brd - The game board (an array of integers)
    :param col - Number of columns in the board
    :param rw_n, the row count of board
    :return
        The well sums. For a well of length n, we define the well sums as
        1 + 2 + 3 + ... + n. This gives more significance to deeper holes.
    """
    wl_s = 0

    # Check for well cells in the "inner columns" of the board.
    # "Inner columns" are the columns that aren't touching the edge of the board.
    for i in range(1, col - 1):
        for j in range(rw_n - 1, -1, -1):
            if (not (brd[j] >> i) & 1) and (brd[j] >> (i - 1) & 1) \
                    and (brd[j] >> (i + 1)) & 1:
                # Found well cell, count it + the number of empty cells below it.
                wl_s += 1

                for k in range(j - 1, -1, -1):
                    if not ((brd[k] >> i) & 1):
                        wl_s += 1
                    else:
                        break

    # Check for well cells in the leftmost col of the board.
    for j in range(rw_n - 1, -1, -1):
        if (not (brd[j] >> 0) & 1) and ((brd[j] >> 1) & 1):
            # Found well cell, count it + the number of empty cells below it.
            wl_s += 1
            for k in range(j - 1, -1, -1):
                if not ((brd[k] >> 0) & 1):
                    wl_s += 1
                else:
                    break

    # Check for well cells in the rightmost col of the board.
    for j in range(rw_n - 1, -1, -1):
        if (not ((brd[j] >> (col - 1)) & 1)) and \
                (brd[j] >> (col - 2) & 1):
            # Found well cell, count it + the number of empty cells below it.
            wl_s += 1
            for k in range(j - 1, -1, -1):
                if not (brd[k] >> (col - 1) & 1):
                    wl_s += 1
                else:
                    break
    return wl_s
