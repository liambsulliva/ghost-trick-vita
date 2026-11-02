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
    char pad90[6];
    unsigned char size;
    char pad97[0x4d];
    struct Line {
        unsigned short width;
        char pad2[6];
    } lines[8];
};

struct SizeInfo {
    unsigned short id;
    unsigned char columns;
    char pad3;
    unsigned short width;
    unsigned short height;
};

struct SpaceInfo {
    unsigned char margin;
    signed char gap;
    signed char shift_one;
    signed char shift_two;
    signed char shift_many;
};

extern "C" {
extern struct SizeInfo data_02081f30[];
extern struct SpaceInfo data_02081f14[];
extern void *func_0200883c(void);
extern void *func_0201a4e8(void *object);
extern unsigned char *func_0201e7f8(void *loader, int index, unsigned int id, unsigned char *dst);
extern void func_0201a97c(Board *board, unsigned char *buffer);
extern void func_0201aa88(Board *board);
void func_0201a3ac(Board *board);
}

void func_0201a3ac(Board *board)
{
    unsigned int width;
    unsigned char i;

    width = board->lines[0].width;
    for (i = 1; i < board->count; i++) {
        if (width < board->lines[i].width) {
            width = board->lines[i].width;
        }
    }
    if (data_02081f30[4].width > width) {
        board->size = 4;
    } else if (data_02081f30[2].width > width) {
        board->size = 2;
    } else {
        board->size = 3;
    }
    func_0201e7f8(func_0201a4e8(func_0200883c()), 0, data_02081f30[board->size].id, board->buffer);
    board->columns = data_02081f30[board->size].columns;
    board->offset_x = (board->s37() - data_02081f30[board->size].width) / 2;
    board->offset_y = (board->s38() - data_02081f30[board->size].height) / 2;
    board->margin = data_02081f14[board->size].margin;
    board->gap = data_02081f14[board->size].gap;
    switch (board->count) {
    case 1:
        board->shift = data_02081f14[board->size].shift_one;
        break;
    case 2:
        board->shift = data_02081f14[board->size].shift_two;
        break;
    default:
        board->shift = data_02081f14[board->size].shift_many;
        board->margin = 1;
        break;
    }
    func_0201a97c(board, board->buffer);
    board->height = data_02081f30[board->size].width;
    func_0201aa88(board);
}
