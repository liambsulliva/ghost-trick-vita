class Actor {
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
    unsigned char pad4[0x24];
    unsigned char sprite[0x88];
    int pos;
    unsigned char padb4[8];
    int tile;
    unsigned char pal;
    unsigned char padc1[0xb];
    void *icon;
    void *mark;
    unsigned int kind;
};

extern "C" {
extern unsigned short data_0209b1b8[];
extern unsigned short data_0209b1ba[];
extern char data_020ce3b8[];
extern void *func_02008444(void);
extern unsigned int func_020283b0(void *a);
extern void func_0200b518(void *a, int b, int c, int d);
extern void func_0200b598(void *a, int b, int c);
extern void func_020280a8(void *a, int b);
extern void *func_0204026c(char *a);
extern void func_020406d8(void *a, int b, int c);
extern int func_0200fb4c(void *a);
extern void func_02014ddc(void *a, int b);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, void *b, int c, int d);
void func_02041220(Actor *p);
}

void func_02041220(Actor *p)
{
    unsigned int n;

    n = func_020283b0(func_02008444());
    p->kind = n;
    if (n < 8) {
        p->tile = data_0209b1b8[n * 2];
        p->pal = data_0209b1ba[n * 2];
    }
    if (p->tile != 0) {
        func_0200b518(p->sprite, 1, 0x80000000, 0x80000000);
        func_0200b598(p->sprite, p->tile, 0xf);
        func_020280a8(p->sprite, p->pal);
        p->pos += 0x14000;
        p->s10();
    }
    p->icon = func_0204026c(data_020ce3b8);
    if (p->icon != 0) {
        func_020406d8(p->icon, 0x12e9, 6);
        func_02014ddc(p->icon, func_0200fb4c(p->icon) + 0x14000);
        func_0202f7e8(func_02009900(), p->icon, 10, 0);
    }
    p->mark = func_0204026c(data_020ce3b8);
    if (p->mark != 0) {
        func_020406d8(p->mark, 0x12f0, 5);
        func_02014ddc(p->mark, func_0200fb4c(p->mark) + 0x14000);
        func_0202f7e8(func_02009900(), p->mark, 10, 0);
    }
}
