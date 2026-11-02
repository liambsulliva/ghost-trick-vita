class Icon {
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
    virtual void s33(int a);
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
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60(int a);
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65(int a);
};

class Body {
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
    virtual int s17();
};

struct PosTable {
    short values[12];
};

struct Vec {
    int x;
    int y;
    int z;
};

struct Panel {
    char pad0[0xb8];
    Body *body;
    Icon *icon;
    int handle;
    unsigned int timer;
    unsigned char slot;
};

extern "C" {
extern const struct PosTable data_0209b778;
extern char data_020cfe10[];
extern Icon *func_02065aa4(char *name);
extern void func_02044d74(Body *body);
extern void func_02044d78(Body *body);
extern void func_020082d8(Vec *vec, int value);
extern void *func_02008444(void);
extern int func_02031b44(void *system);
extern void func_020153d8(Icon *icon, Panel *panel);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, void *b, int c, int d);
extern int func_02010b54(Panel *panel);
extern int func_02031af4(void *system, int id);
void func_0204b128(Panel *panel);
}

void func_0204b128(Panel *panel)
{
    struct PosTable pos;
    Vec vec;

    if (++panel->timer > 0x10 && panel->handle < 0 && panel->icon == 0) {
        pos = data_0209b778;
        panel->icon = func_02065aa4(data_020cfe10);
        func_02044d74(panel->body);
        func_02044d78(panel->body);
        func_020082d8(&vec, panel->body->s17());
        panel->handle = func_02031b44(func_02008444());
        panel->icon->s65(0xcf);
        panel->icon->s13(pos.values[panel->slot * 2] << 12, pos.values[panel->slot * 2 + 1] << 12, 0);
        panel->icon->s33(0x140);
        panel->icon->s60(panel->handle);
        func_020153d8(panel->icon, panel);
        func_0202f7e8(func_02009900(), panel->icon, 7, 0);
    }
    if (func_02010b54(panel) != 0) {
        panel->body->s10();
        if (func_02031af4(func_02008444(), panel->handle) == 0 && panel->icon != 0) {
            panel->icon->s10();
        }
    } else {
        panel->body->s11();
        if (func_02031af4(func_02008444(), panel->handle) == 0 && panel->icon != 0) {
            panel->icon->s11();
        }
    }
}
