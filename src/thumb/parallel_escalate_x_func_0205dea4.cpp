class Part {
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
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
};

class Stage {
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
    virtual void s33(int value);
    unsigned char pad4[0x44];
    unsigned short mode;
    unsigned short count;
    unsigned char ready;
    unsigned char busy;
    unsigned char kind;
    unsigned char extra;
    unsigned char quiet;
    unsigned char pad51[0x3];
    unsigned char loaded;
    unsigned char pad55[0x3];
    int width;
    int height;
    int step;
    int x;
    int speed;
    int y;
    Part *frame;
    Part *board;
    Part *glow;
    Part *marker;
    Part *badge;
};

extern "C" {
extern char data_020cff28[];
extern char data_020cfa80[];
extern char data_020cfa9c[];
extern char data_020cfad4[];
extern char data_020cfb0c[];
extern char data_020cfa64[];
extern void *func_02009900(void);
extern void *func_0200847c(void);
extern void func_0202fad4(void *system, char *name);
extern Part *func_0202fb48(void *system, char *name, int layer);
extern void func_0202f7e8(void *system, Part *part, int a, int b);
extern void func_020153d8(Part *part, Stage *owner);
extern Part *func_0205bf90(char *name);
extern Part *func_0205bfa4(char *name);
extern Part *func_0205bff4(char *name);
extern Part *func_0205bfe0(char *name);
extern Part *func_0205c008(char *name);
extern void func_0205e0c4(Part *part, unsigned char value);
extern int func_0205e0cc(Part *part);
extern void func_0205e0d4(Part *part, int value);
extern void func_0205e0dc(Part *part, int a, unsigned char b);
extern void func_0205e0e8(Part *part, unsigned short value);
extern int func_0205e0f0(Part *part);
extern void func_0205d73c(Part *part);
extern void func_0205d534(Part *part);
extern void func_0205e0f8(Part *part, int value);
extern void func_0205e100(Part *part, int a, unsigned char b);
extern void func_0205e2dc(Stage *p);
extern void func_0202a0c0(void *sound, int id);
void func_0205dea4(Stage *p);
}

void func_0205dea4(Stage *p)
{
    func_0202fad4(func_02009900(), data_020cff28);
    p->busy = 0;
    p->ready = 0;
    p->count = 0;
    p->s33(0x620);
    p->frame = func_0205bf90(data_020cfa80);
    if (p->frame != 0) {
        func_020153d8(p->frame, p);
        func_0205e0c4(p->frame, p->kind);
        func_0202f7e8(func_02009900(), p->frame, 7, 0);
    }
    p->board = func_0202fb48(func_02009900(), data_020cfa9c, 7);
    if (p->board != 0) {
        if (func_0205e0cc(p->board) != 0) {
            func_020153d8(p->board, p);
            func_0205e0d4(p->board, 0);
            p->loaded = 1;
        } else {
            p->board->s41();
            p->board = 0;
        }
    }
    if (p->board == 0) {
        p->board = func_0205bfa4(data_020cfa9c);
        if (p->board != 0) {
            func_020153d8(p->board, p);
            func_0202f7e8(func_02009900(), p->board, 7, 0);
        }
    }
    p->marker = func_0205bff4(data_020cfad4);
    if (p->marker != 0) {
        func_0205e0dc(p->marker, 0, p->quiet);
        func_0205e0e8(p->marker, p->mode);
        func_020153d8(p->marker, p);
        func_0202f7e8(func_02009900(), p->marker, 7, 0);
    }
    p->badge = func_0202fb48(func_02009900(), data_020cfb0c, 7);
    if (p->badge != 0) {
        if (func_0205e0f0(p->badge) != 0) {
            func_020153d8(p->badge, p);
            func_0205d73c(p->badge);
            func_0205d534(p->badge);
            func_0205e0f8(p->badge, 0);
            p->loaded = 1;
        } else {
            p->badge = 0;
        }
    }
    if (p->badge == 0) {
        p->badge = func_0205bfe0(data_020cfb0c);
        if (p->badge != 0) {
            func_020153d8(p->badge, p);
            func_0202f7e8(func_02009900(), p->badge, 7, 0);
        }
    }
    func_0205e2dc(p);
    if (p->extra != 0) {
        p->glow = func_0205c008(data_020cfa64);
        if (p->glow != 0) {
            func_0205e100(p->glow, 0x7fff, p->quiet);
            func_020153d8(p->glow, p);
            func_0202f7e8(func_02009900(), p->glow, 7, 0);
        }
    }
    p->width = 0x258;
    p->height = 0x8c;
    p->step = 8;
    p->x = 0;
    p->speed = 0x2000;
    p->y = -0x19a;
    if (p->quiet == 0 && p->loaded == 0) {
        func_0202a0c0(func_0200847c(), 0xe);
    }
}
