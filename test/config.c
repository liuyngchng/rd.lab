#include <math.h>
#include "config.h"

struct config get_config()
{
    struct config config;
    config.base_url = "http://127.0.0.1:8000/";
    config.config_file = "config.txt";
    config.col = 10;
    config.row = 20;
    config.pce_col = 4;
    config.pce_row = 4;
    config.new_pce_pos[0] = -3;
    config.new_pce_pos[1] = 1;
    config.col_bd_msk = pow(2, config.col - 1);
    config.col_max = pow(2, config.col) - 1;

    config.pcs[0] = (struct rot_piece) {
        4, 2, 0,
        0b0010,
        0b0111,
        0b0000,
        0b0000,

        0b0010,
        0b0110,
        0b0010,
        0b0000,

        0b0000,
        0b0111,
        0b0010,
        0b0000,

        0b0010,
        0b0011,
        0b0010,
        0b0000
    };
    config.pcs[1] = (struct rot_piece) {
        4, 0, 0,

        0b0000,
        0b0110,
        0b0110,
        0b0000,

        0b0000,
        0b0110,
        0b0110,
        0b0000,

        0b0000,
        0b0110,
        0b0110,
        0b0000,

        0b0000,
        0b0110,
        0b0110,
        0b0000
    };
    config.pcs[2] = (struct rot_piece) {
        4, 1, 0,

        0b0000,
        0b1111,
        0b0000,
        0b0000,

        0b0100,
        0b0100,
        0b0100,
        0b0100,

        0b0000,
        0b0000,
        0b1111,
        0b0000,

        0b0010,
        0b0010,
        0b0010,
        0b0010
    };
    return config;
}
