struct Field;

struct Board {
    char pad0[0x36];
    unsigned char flag;
    struct Field *field;
};

extern int func_0200e478(struct Field *field, unsigned int id);

unsigned char func_0201cfd0(struct Board *board, unsigned int id)
{
    if (board->flag != 0) {
        if (board->field != 0) {
            return func_0200e478(board->field, id);
        }
    }
    return 0;
}
