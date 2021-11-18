#ifndef CONFIG_H
#define CONFIG_H

struct rot_piece
{
    int r_lmt;          // roate limit
    int type;           // piece type
    int r;              // rotate index
    int piece[4][4];    // piece data in all rotate direction

};

struct config
{
    char *base_url;
    char *config_file;
    struct rot_piece pcs[4];
    int col;
    int row;
    int pce_col;
    int pce_row;
    int col_bd_msk;
    int col_max;
    int new_pce_pos[2];
};

struct config get_config();

#endif
