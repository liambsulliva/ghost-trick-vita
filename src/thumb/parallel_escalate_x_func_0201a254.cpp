class Board {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual int s37();
    virtual int s38();

    char pad4[0x2c];
    short offset_x;
    short offset_y;
    char pad34[4];
    unsigned short count;
    char pad3a[6];
    short margin;
    short gap;
    short shift;
    unsigned short height;
    char pad48[0x20];
    unsigned char *buffer;
    char pad6c[0x20];
    int columns;
    int mode;
    char pad94[2];
    unsigned char size;
    char pad97;
    struct Page {
        char pad0[0xc];
        int offset;
        int value;
        char pad14[0x10];
    } pages[2];
    char pade0[4];
    struct Line {
        unsigned short width;
        char pad2[6];
    } lines[8];
};

struct SpaceInfo {
    unsigned char margin;
    signed char gap;
    signed char shift_one;
    signed char shift_two;
    signed char shift_many;
};

extern "C" {
extern struct SpaceInfo data_02081f14[];
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern void func_0201786c(void *object, int value);
extern void func_0201a680(Board *board, unsigned char *buffer, unsigned char rows, unsigned short count);
extern void func_0201a97c(Board *board, unsigned char *buffer);
extern void func_0201aa88(Board *board);
void func_0201a254(Board *board);
}

void func_0201a254(Board *board)
{
    unsigned int width;
    unsigned char i;
    signed char rows;
    int page;

    board->mode = 2;
    if (board->size == 0) {
        board->mode = 1;
    } else if (board->size == 1) {
        board->mode = 0;
    }
    board->offset_x = 0x12;
    board->margin = data_02081f14[board->size].margin;
    board->gap = data_02081f14[board->size].gap;
    switch (board->count) {
    case 1:
        board->offset_y = 0xc;
        board->shift = data_02081f14[board->size].shift_one;
        break;
    case 2:
        board->offset_y = 4;
        board->shift = data_02081f14[board->size].shift_two;
        break;
    default:
        board->offset_y = 4;
        board->shift = data_02081f14[board->size].shift_many;
        board->margin = 1;
        break;
    }
    width = board->lines[0].width;
    for (i = 1; i < board->count; i++) {
        if (width < board->lines[i].width) {
            width = board->lines[i].width;
        }
    }
    rows = (int)width / 6 + 1;
    if (rows > 0x18) {
        rows = 0x18;
    }
    page = 0;
    if (rows * 6 > 0x60) {
        page = 1;
        board->columns = 2;
        board->offset_x = board->offset_x + 0x40;
    } else {
        board->columns = 1;
    }
    board->offset_x = board->offset_x - (short)((rows - 10) * 6 / 2);
    func_0207303c(&board->pages[page], board->buffer, 0x24);
    func_0201786c(board->buffer + board->pages[page].offset, board->pages[1].value);
    func_0201a680(board, board->buffer + 0x24, rows, board->count);
    func_0201a97c(board, board->buffer);
    board->height = rows * 6;
    func_0201aa88(board);
}
