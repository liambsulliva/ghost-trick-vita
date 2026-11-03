typedef struct Cmd {
    int type;
    int a1;
    int a2;
    int a3;
    int a4;
    int a5;
    int a6;
    int a7;
} Cmd;

extern Cmd *func_020074a0(void);
extern void func_02007958(int a);
extern void func_0200796c(int a);
extern void func_02003d0c(unsigned short a, unsigned char b);
extern void func_02004814(short a, unsigned short b, int c, unsigned char d, unsigned char e, short f, short g, signed char h, short i, unsigned short j, unsigned char k);
extern void func_02004a84(short a, short b, unsigned short c, int d, unsigned char e, unsigned char f, short g, short h, signed char i, short j, unsigned short k, unsigned char l);
extern void func_020040fc(short a, short b, short c, unsigned short d, int e, unsigned char f, unsigned char g, short h, short i, signed char j, short k, unsigned char l, int m);
extern void func_02004050(int a, int b, int c, int d, unsigned short e, unsigned char f);
extern void func_02006990(short a, short b, short c, int d, int e, unsigned char f);
extern void func_02004234(int a, int b, int c, int d, int e);
extern void func_02004360(int a, int b, int c);
extern void func_02007a24(int a, unsigned short b, int c);
extern void func_02007a44(int a, unsigned short b, int c);
extern void func_02007a3c(int a, unsigned short b, int c);
extern void func_02004d24(int a, short b, short c, unsigned short d, int e, unsigned char f, unsigned char g, short h, short i, signed char j, short k, unsigned short l, unsigned char m);
extern void func_020044ec(int a, int b, int c, unsigned char d, unsigned char e, unsigned short f, unsigned char g);
extern void func_020043ac(int a, short b, short c, unsigned char d, unsigned char e, unsigned short f, int g, unsigned char h, unsigned char i, short j, short k, signed char l, short m, unsigned char n);
extern void func_02004598(int a, int b, int c, unsigned char d, unsigned char e, int f, int g, unsigned char h);
extern void func_020046d0(int a, int b, int c, unsigned char d, unsigned char e, int f, int g);
extern void func_02005cec(int a, int b, int c, int d, int e, int f);
extern void func_0200611c(int a, int b);
extern void func_02006358(int a, int b);
extern void func_02006444(int a, int b, int c);
extern void func_020063c8(int a, unsigned char b, int c);
extern void func_02005f7c(int a, int b);
extern void func_020068c0(int a, int b);
extern void func_02006938(int a);

void func_020074ec(void)
{
    int n;
    Cmd *c;

    for (n = 0; n < 32; n++) {
        c = func_020074a0();
        if (c == 0) {
            return;
        }
        switch (c->type) {
        case 1:
            func_02007958(c->a1);
            break;
        case 2:
            func_0200796c(c->a1);
            break;
        case 3:
            func_02003d0c(c->a1, c->a2);
            break;
        case 4:
            func_02004814(c->a1 >> 16, c->a2, c->a3, c->a4 >> 16, c->a4 >> 8, c->a6 >> 16, c->a6, c->a4, c->a5 >> 16, c->a5, c->a4 >> 24);
            break;
        case 5:
            func_02004a84(c->a1 >> 16, c->a1, c->a2, c->a3, c->a4 >> 16, c->a4 >> 8, c->a6 >> 16, c->a6, c->a4, c->a5 >> 16, c->a5, c->a4 >> 24);
            break;
        case 9:
            func_020040fc(c->a1 >> 16, c->a1, c->a2 >> 16, c->a2, c->a3, c->a4 >> 8, c->a4, c->a6 >> 16, c->a6, c->a5 >> 16, c->a5, c->a5 >> 24, 0);
            break;
        case 10:
            func_020040fc(c->a1 >> 16, c->a1, c->a2 >> 16, c->a2, c->a3, c->a4 >> 8, c->a4, c->a6 >> 16, c->a6, c->a5 >> 16, c->a5, c->a5 >> 24, 1);
            break;
        case 7:
            func_02004050(c->a1, c->a2, c->a3, c->a4, c->a5, c->a6);
            break;
        case 15:
        case 16:
            func_02006990(c->a1 >> 16, c->a1, c->a2 >> 16, c->a3, c->a4, c->a5);
            break;
        case 13:
            func_02004234(c->a1, c->a2, -1, c->a3, c->a4);
            break;
        case 19:
            func_02004360(c->a1, c->a2, c->a3);
            break;
        case 20:
            func_02007a24(c->a1, c->a2, c->a3);
            break;
        case 21:
            func_02007a44(c->a1, c->a2, c->a3);
            break;
        case 22:
            func_02007a3c(c->a1, c->a2, c->a3);
            break;
        case 24:
            func_02004d24(c->a7, c->a1 >> 16, c->a1, c->a2, c->a3, c->a4 >> 16, c->a4 >> 8, c->a6 >> 16, c->a6, c->a4, c->a5 >> 16, c->a5, c->a4 >> 24);
            break;
        case 25:
            func_020044ec(c->a1, c->a2, c->a3, c->a4 >> 8, c->a4, c->a5, c->a6);
            break;
        case 26:
            func_020043ac(c->a7, c->a1, c->a2 >> 16, c->a4 >> 24, c->a4 >> 16, c->a2, c->a3, c->a4 >> 8, c->a4, c->a6 >> 16, c->a6, c->a5 >> 16, c->a5, c->a5 >> 24);
            break;
        case 27:
            func_02004598(c->a1, c->a2, c->a3, c->a4 >> 8, c->a4, c->a5, c->a6, c->a7);
            break;
        case 28:
            func_020046d0(c->a1, c->a2, c->a3, c->a4 >> 8, c->a4, c->a5, c->a6);
            break;
        case 6:
            func_02005cec(c->a1, c->a2, c->a3, c->a4, c->a5, c->a6);
            break;
        case 8:
            func_0200611c(c->a1, c->a2);
            break;
        case 11:
            func_02006358(c->a1, c->a2);
            break;
        case 17:
            func_02006444(c->a1, c->a2, c->a3);
            break;
        case 12:
            func_020063c8(c->a1, c->a2, c->a3);
            break;
        case 23:
            func_02005f7c(c->a1, c->a2);
            break;
        case 18:
            func_020068c0(c->a1, c->a2);
            break;
        case 14:
            func_02006938(c->a1);
            break;
        }
    }
}
