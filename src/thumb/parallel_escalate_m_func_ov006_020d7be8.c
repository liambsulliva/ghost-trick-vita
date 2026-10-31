struct Board {
    char pad00[0x20];
    int busy;
    char pad24;
    unsigned char flags;
    char pad26[5];
    unsigned char state;
    char pad2c[6];
    signed char sel[2];
    char pad34[4];
    signed char cells[2][0x32];
    unsigned int values[2][50];
    char pad22c[0x190];
    unsigned char used[0x6c];
    void *task;
};

extern void func_ov006_020d4098(void *object, unsigned char bit, signed char cell, unsigned char flag, unsigned int value);
extern void func_ov006_020d420c(void *object);
extern void func_ov006_020d7108(struct Board *board, void *object, int mode);
extern int func_ov006_020d74c8(struct Board *board, unsigned char bit, signed char cell);
extern void *func_ov006_020d813c(int id, int a, int b);
extern void func_02014de4(void *task, int value);

void func_ov006_020d7be8(struct Board *board, void *object)
{
    unsigned char bit;
    signed char cell;
    unsigned char flag;

    bit = board->flags & 1;
    cell = board->cells[bit][board->sel[bit]];
    flag = 0;
    if (bit != 0 && !board->used[cell]) {
        flag = 1;
    }
    func_ov006_020d4098(object, bit, cell, flag, board->values[bit][cell]);
    func_ov006_020d420c(object);
    board->state = 0;
    func_ov006_020d7108(board, object, 1);
    if (board->task == 0) {
        if (func_ov006_020d74c8(board, bit, cell) != 0) {
            if (board->busy == 0) {
                board->task = func_ov006_020d813c(0x8008, 3, 2);
                func_02014de4(board->task, 0x5e000);
            }
        }
    }
}
