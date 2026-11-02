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
    virtual void s52(int a);
    virtual void s53(int a);
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

class Res {
public:
    virtual void r0();
    virtual void r1();
    virtual void *r2(int size, void *name);
    virtual void r3();
    virtual void r4(void *ptr);
};

struct Text {
    unsigned char pad0[0x14];
    int off;
    int size;
};

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
    virtual void s33(int a);
    unsigned char pad04[0xd0];
    Icon *icon;
    int handle;
    void *label;
    unsigned char flags;
    unsigned char pade1[0xf];
    unsigned short sound;
    unsigned char padf2[0xe];
    unsigned short *pal;
    unsigned int count;
};

extern "C" {
extern char data_020cfe80[];
extern char data_0209b518[];
extern void *func_0201003c(void);
extern struct Text *func_02015c18(void *mgr, int kind, int id);
extern Icon *func_02065b30(char *name);
extern void *func_02008444(void);
extern int func_02031b44(void *game);
extern void func_020153d8(Icon *icon, Board *owner);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, void *obj, int a, int b);
extern void func_02015c30(void *mgr);
extern void func_02015c44(void *mgr);
extern void *func_02031254(int size);
extern void *func_02042718(void *mem);
extern void func_020427b0(void *label, void *text, int a, int b, int c, int d, int e, int f, int g, int h, int i);
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *system);
extern void func_02044d7c(Board *p, int a, int b);
extern int func_02044d74(Board *p);
extern int func_02044d78(Board *p);
extern void func_0205700c(Board *p, int a, int b, int c, int d);
extern unsigned short func_02015c58(void *mgr);
void func_02044b48(Board *p, int id);
}

void func_02044b48(Board *p, int id)
{
    struct Text *text;
    unsigned short *src;
    unsigned int i;
    void *label;
    int w;
    int h;

    text = 0;
    if (id != 0) {
        text = func_02015c18(func_0201003c(), ((unsigned int)p->flags << 24) >> 27, id);
    } else {
        p->icon = func_02065b30(data_020cfe80);
        if (p->icon != 0) {
            p->flags |= 2;
            if ((((unsigned int)p->flags << 24) >> 27) == 4) {
                p->icon->s65(0x8c);
            } else {
                p->icon->s65(0x85);
            }
            p->handle = func_02031b44(func_02008444());
            p->icon->s53(1);
            p->icon->s60(p->handle);
            p->icon->s33(0x571);
            func_020153d8(p->icon, p);
            if ((((unsigned int)p->flags << 24) >> 27) == 3 || (((unsigned int)p->flags << 24) >> 27) == 6) {
                p->icon->s52(1);
            }
            func_0202f7e8(func_02009900(), p->icon, 7, 0);
            return;
        }
    }
    func_02015c30(func_0201003c());
    func_02015c44(func_0201003c());
    if (text != 0) {
        if (p->label == 0) {
            label = func_02031254(0x54);
            if (label != 0) {
                label = func_02042718(label);
            }
            p->label = label;
        }
        func_020427b0(p->label, text, 2, 0, 1, 1, 0xffff, 1, -1, -1, 0x100);
        src = (unsigned short *)((char *)text + text->off);
        p->count = text->size;
        if (p->pal != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->r4(p->pal);
                p->pal = 0;
            }
        }
        p->pal = (unsigned short *)func_02007b70(func_02007bd8())->r2(p->count << 1, data_0209b518);
        for (i = 0; i < p->count >> 1; i++) {
            p->pal[i] = src[i];
            p->pal[i + (p->count >> 1)] = src[3];
        }
        func_02044d7c(p, 0, p->count >> 1);
        w = func_02044d74(p);
        h = func_02044d78(p);
        func_0205700c(p, -(w >> 1), h >> 1, w >> 1, -(h >> 1));
        p->sound = func_02015c58(func_0201003c());
    }
    p->s33(0x571);
}
