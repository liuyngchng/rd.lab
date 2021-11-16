#include <math.h>
#include <stdio.h>

struct piece
{
    int r_lmt;          // roate limit
    int type;           // piece type
    int r;              // rotate index
    int **a;            // piece data in all rotate direction

};

struct config
{
    char *base_url;
    char *config_file;
    struct piece *pieces;
    int pce_col;
    int pce_row;
    int *new_pce_pos;
    int col_bd_msk;
    int col_max;
};
//{
//    char *base_url = 'http://127.0.0.1:8000/'
//    char *config_file = 'config.txt'
//    struct piece pieces[3]
//    int pieces[3][][] = [
//            [
//                  'r_lmt': 4, 'type': 2, 'r': 0,
//                 'piece': [
//                     [0b0010,
//                      0b0111,
//                      0b0000,
//                      0b0000
//                      ],
//
//                     [
//                        0b0010,
//                        0b0110,
//                        0b0010,
//                        0b0000
//                    ],
//
//                     [0b0000,
//                      0b0111,
//                      0b0010,
//                      0b0000],
//
//                     [0b0010,
//                      0b0011,
//                      0b0010,
//                      0b0000]
//                 ]
//            ],
//            [
//                 'r_lmt': 4, 'type': 0, 'r': 0,
//                 'piece': [
//
//                     [0b0000,
//                      0b0110,
//                      0b0110,
//                      0b0000],
//
//                     [0b0000,
//                      0b0110,
//                      0b0110,
//                      0b0000],
//
//                     [0b0000,
//                      0b0110,
//                      0b0110,
//                      0b0000],
//
//                     [0b0000,
//                      0b0110,
//                      0b0110,
//                      0b0000]
//                 ]
//            ],
//            [
//                 'r_lmt': 4, 'type': 1, 'r': 0,
//                 'piece': [
//                     [0b0000,
//                      0b1111,
//                      0b0000,
//                      0b0000],
//
//                     [0b0100,
//                      0b0100,
//                      0b0100,
//                      0b0100],
//
//                     [0b0000,
//                      0b0000,
//                      0b1111,
//                      0b0000],
//
//                     [0b0010,
//                      0b0010,
//                      0b0010,
//                      0b0010]
//                 ]
//             ]
//        ]
//
//        # positions
//        int col = 10;                        // ctx col
//        int row = 20;                        // ctx row
//        self.pce_col = 4                        # piece col
//        self.pce_row = 4                        # piece row
//
//        # 新出现的形状所在位置坐标
//        self.new_pce_pos = []                               # 新出现图形的坐标
//        self.col_bd_msk = pow(2, self.col - 1)      # col border mask
//        self.col_max = pow(2, self.col) - 1

//}

void get_config()
{
    struct piece p[4];
//    piece[0].r_lmt = 4;
    int a[4][4]= {{1,2,3,4}, {1,2,3,4},{1,2,3,4},{1,2,3,4}};
    p[0] = (struct piece){4, 2, 0};
    p[0].a = a;
    printf("p[0].a[0][1]=%d", (p[0].a)[0][1]);

//    int **b;
//    b =a;
//    struct config config;
//    config.base_url = "http://127.0.0.1:8000/";
//    config.config_file = "config.txt";
//    struct test ts[2];
//    config.pieces = [piece];
//    config.pce_col = 4;
//    config.pce_row = 4;
//    config.new_pce_pos = [-3, 1];
//    config.col_bd_msk = pow(2, self.col - 1);
//    config.col_max = pow(2, self.col) - 1;
}


int main()
{
    get_config();
    return 0;
}
