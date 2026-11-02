typedef struct Entry {
    int words[14];
} Entry;

typedef struct Panel {
    char pad0[0x86];
    unsigned char f86;
    unsigned char f87;
    int f88;
    int f8c;
    int f90;
    int f94[4];
    int fa4[14];
    unsigned int count;
    int pade0;
    int idx[8];
    int first[8];
    int second[8];
} Panel;

extern char data_020854ac;
extern char data_020854bc;
extern char data_020854c4;
extern char data_020854d0;
extern char data_020854e0;
extern char data_020854f0;
extern char data_02085500;
extern char data_02085510;
extern char data_02085524;
extern char data_02085538;
extern void func_0202b588(Panel *p, void *list, int a, int b);
extern int func_02012ea8();
extern int func_020089c8();
extern int func_0202d650();
extern int func_0200b374();
extern void func_0202cffc(void *dst, int value, int size);
extern int func_0202d684(Panel *p, unsigned int i);
extern int func_0202d688(Panel *p, unsigned int i);
extern int func_0202d68c(Panel *p, unsigned int i);
extern int func_0202d6a4(Panel *p, unsigned int i);
extern void func_02008a00();
extern void func_020089fc();

void func_0202d428(Panel *p, void *list, int a, int b)
{
    Entry e1;
    Entry e2;
    Entry e3;
    Entry e4;
    Entry e5;
    Entry e6;
    Entry e7;
    Entry e8;
    Entry e9;
    Entry e10;
    unsigned int n;
    unsigned int i;
    int x0;
    int y0;
    int x;
    int y;

    func_0202b588(p, list, a, b);
    func_02012ea8(&e1, p, (int)&data_020854ac, &p->f86, 1, 0);
    func_02008a00(list, &e1);
    func_020089fc(&e1);
    func_020089c8(&e2, p, (int)&data_020854bc, &p->f88, 1, 0);
    func_02008a00(list, &e2);
    func_020089fc(&e2);
    func_020089c8(&e3, p, (int)&data_020854c4, &p->f8c, 1, 0);
    func_02008a00(list, &e3);
    func_020089fc(&e3);
    func_0202d650(&e4, p, (int)&data_020854d0, &p->f87, 1, 0);
    func_02008a00(list, &e4);
    func_020089fc(&e4);
    func_02012ea8(&e5, p, (int)&data_020854e0, &p->f90, 0x21, 4);
    func_02008a00(list, &e5);
    func_020089fc(&e5);
    func_0200b374(&e6, p, (int)&data_020854f0, p->f94, 0x21, 4);
    func_02008a00(list, &e6);
    func_020089fc(&e6);
    func_0200b374(&e7, p, (int)&data_02085500, p->fa4, 0x21, 4);
    func_02008a00(list, &e7);
    func_020089fc(&e7);
    func_0202cffc(p->idx, -1, 0x20);
    func_0202cffc(p->first, 0, 0x20);
    func_0202cffc(p->second, 0, 0x20);
    n = 0;
    for (i = 0; i < p->count; i++) {
        x0 = func_0202d684(p, i);
        y0 = func_0202d688(p, i);
        x = func_0202d68c(p, i);
        y = func_0202d6a4(p, i);
        if ((x != x0 || y != y0) && n < 8) {
            p->idx[n] = i;
            p->first[n] = x;
            p->second[n] = x;
            n++;
        }
    }
    func_0200b374(&e8, p, (int)&data_02085510, p->idx, 0x21, 8);
    func_02008a00(list, &e8);
    func_020089fc(&e8);
    func_020089c8(&e9, p, (int)&data_02085524, p->first, 0x21, 8);
    func_02008a00(list, &e9);
    func_020089fc(&e9);
    func_020089c8(&e10, p, (int)&data_02085538, p->second, 0x21, 8);
    func_02008a00(list, &e10);
    func_020089fc(&e10);
}
