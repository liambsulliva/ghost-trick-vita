struct State020339dc {
    unsigned char pad[4];
    unsigned char state;
    unsigned char wait;
};

extern unsigned int func_0201572c(void);
extern unsigned int *func_02008444(void);
extern void func_0200fe74(unsigned int *p);
extern unsigned char *func_02009900(void);
extern unsigned int func_0202fa8c(unsigned char *p, unsigned char *d);
extern void func_02011bfc(unsigned int *p);
extern void func_020521d4(unsigned char *d, int a, int b, int c);
extern void func_02010384(unsigned int *p, int a);
extern void func_0200da04(unsigned char *a, unsigned char *b, unsigned char *c, unsigned char *d);
extern int func_02011264(void);
extern int func_0201123c(int a, int b);
extern void func_02022f2c(int a, unsigned char *b, int c);
extern unsigned char data_020cedc4[];
extern unsigned char data_020cee88[];
extern unsigned char data_020cf898[];
extern unsigned char data_020cfe2c[];
extern unsigned char data_020ce900[];
extern unsigned char data_02099dc8[];
extern unsigned char data_02099dfc[];
extern unsigned char data_02099dd4[];
extern unsigned char data_0209a038[];
extern unsigned char data_02099e00[];
extern unsigned char data_020cd7d0[];

void func_020339dc(struct State020339dc *s)
{
    switch (s->state) {
    case 0:
        if (func_0201572c() == 0) {
            func_0200fe74(func_02008444());
            s->state++;
            s->wait = 60;
        }
        break;
    case 1:
        s->wait--;
        if (s->wait != 0) {
            if (func_0202fa8c(func_02009900(), data_020cedc4) != 0) return;
            if (func_0202fa8c(func_02009900(), data_020cee88) != 0) return;
            if (func_0202fa8c(func_02009900(), data_020cf898) != 0) return;
            if (func_0202fa8c(func_02009900(), data_020cfe2c) != 0) return;
            if (func_0202fa8c(func_02009900(), data_020ce900) != 0) return;
        }
        func_02011bfc(func_02008444());
        func_020521d4(data_02099dc8, 0, 15, 0x100);
        s->state++;
        break;
    case 2:
        if (func_0201572c() == 0) {
            func_02010384(func_02008444(), 0);
            func_0200da04(data_02099dfc, data_02099dd4, data_0209a038, data_02099e00);
            func_02022f2c(func_0201123c(func_02011264(), 0), data_020cd7d0, 0);
        }
        break;
    }
}
