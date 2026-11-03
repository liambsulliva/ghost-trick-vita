class Obj {
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
    virtual void s13(int x, int y, int z);
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
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int a, int b);
};

struct Lane {
    char pad00[0x28];
    int x;
    int y;
    char pad30[0x10];
    unsigned short state;
    char pad42[0x46];
    unsigned short id;
    char pad8a[0x2a];
    unsigned char flipped;
    char padb5[7];
    Obj *sides[2];
    Obj *helper;
    char padc8[0xc];
    int speed_x;
    int speed_y;
    int fdc;
    int fe0;
    int distance;
    char pade8[1];
    unsigned char special;
};

extern "C" {
extern char data_020cea88;
extern char data_020cf724;
extern int func_0206ef28(int a, int b);
extern void *func_02008444(void);
extern int func_0200f5c8(void *game);
extern Obj *func_02046ff4(void *type);
extern Obj *func_0205693c(void *type);
extern void func_020153d8(Obj *object, struct Lane *lane);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, Obj *b, int c, int d);
extern void func_0200ee34(Obj *object, int r, int g, int b);

void func_02049538(struct Lane *lane)
{
    int x;
    unsigned int i;

    if (lane->id == 0x801c) {
        lane->special = 1;
    }
    if (lane->state == 0) {
        lane->speed_x = func_0206ef28(lane->x, 0x10000);
        lane->speed_y = 0;
        lane->distance = 0x80000 - lane->x;
        if (lane->flipped != 0) {
            lane->distance = lane->x < 0 ? -lane->x : lane->x;
        }
        if (lane->y != 0) {
            if (lane->flipped != 0) {
                lane->speed_x = func_0206ef28(func_0200f5c8(func_02008444()) - lane->y, 0x10000);
            } else {
                lane->speed_x = func_0206ef28(func_0200f5c8(func_02008444()) - (lane->y - 0x80000), 0x10000);
            }
            lane->distance = lane->y - func_0200f5c8(func_02008444());
        }
    } else {
        if (lane->flipped != 0) {
            lane->speed_x = func_0206ef28(func_0200f5c8(func_02008444()) - lane->x, 0x10000);
        } else {
            lane->speed_x = func_0206ef28(func_0200f5c8(func_02008444()) - (lane->x - 0x80000), 0x10000);
        }
        lane->distance = lane->x - func_0200f5c8(func_02008444());
        lane->speed_y = func_0206ef28(lane->y, 0x10000);
    }
    lane->fdc = func_0206ef28(0x80000, 0x10000);
    lane->fe0 = func_0206ef28(0x60000, 0x10000);
    lane->helper = func_02046ff4(&data_020cea88);
    if (lane->helper != 0) {
        lane->helper->s11();
        func_020153d8(lane->helper, lane);
        func_0202f7e8(func_02009900(), lane->helper, 7, 0);
    }
    for (i = 0; i < 2; i++) {
        lane->sides[i] = func_0205693c(&data_020cf724);
        if (lane->sides[i] != 0) {
            lane->sides[i]->s33(0x471);
            lane->sides[i]->s46(0x80000, 0x60000);
            func_0200ee34(lane->sides[i], 0, 0, 0);
            lane->sides[i]->s11();
            x = -0xc4000;
            if (lane->flipped != 0) {
                x = 0xc4000;
            }
            lane->sides[i]->s13(x, (0x30 - i * 0x60) << 12, 0);
            func_020153d8(lane->sides[i], lane);
            func_0202f7e8(func_02009900(), lane->sides[i], 7, 0);
        }
    }
}
}
