struct Field;
struct Board {
    unsigned char pad[0x38];
    struct Field *field;
};
extern const char data_02084a52[];
extern const char data_02084a5e[];
extern const char data_02084a44[];
extern int func_0201cf74(struct Field *field);
extern int func_0201d380(struct Board *board, unsigned int cell, const char *table);

int func_0201d2f4(struct Board *board, unsigned int x, unsigned int y)
{
    int result;

    result = 0;
    if (x > 0xe0 || y > 0xe0 || x == 0x3e || x == 0x3f || (unsigned short)(x + 0xff8d) <= 1) {
        if (y != 0xff) {
            result = 1;
        }
    } else if (board->field != 0 && func_0201cf74(board->field) == 0) {
        if (func_0201d380(board, x, data_02084a52) != 0 || func_0201d380(board, y, data_02084a52) != 0 ||
            func_0201d380(board, x, data_02084a5e) != 0 || func_0201d380(board, x, data_02084a44) != 0) {
            result = -1;
        }
    }
    return result;
}
