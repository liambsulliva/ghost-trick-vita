struct Obj759c {
    unsigned char pad00[0x20];
    int w20;
    unsigned char pad24[1];
    unsigned char flags;
    unsigned char pad26[0xc];
    signed char cur[2];
    unsigned char pad34[4];
    signed char list[2][0x32];
    int vals[2][50];
    unsigned char pad22c[0x20c];
    int arrA[2][33];
    int arrB[2][33];
};

extern int func_ov006_020d41c8(int, int, unsigned char, int);
extern int func_ov006_020d813c(int, int, int);
extern void func_ov006_020d6f80(int, unsigned int);
extern void func_02014de4(int, int);
extern void func_ov006_020d76c8(int, int);
extern void func_ov006_020d76e0(int);
extern void func_ov006_020d76f4(int);
extern int data_020d8a48[];

void func_ov006_020d759c(struct Obj759c *param_1, int param_2, int param_3, signed char param_4, int param_5)
{
    int *tab[2];
    unsigned int bit;
    signed char *row;
    int *v;
    int s;
    int *a;
    int *b;
    int t;

    tab[0] = (int *)data_020d8a48[0];
    tab[1] = (int *)data_020d8a48[1];
    bit = ((unsigned int)param_1->flags << 23) >> 24;
    row = param_1->list[param_3];
    s = row[param_4];
    v = param_1->vals[param_3];
    t = (signed char)func_ov006_020d41c8(param_2, param_3, (unsigned char)s, v[s]);
    b = param_1->arrB[param_3];
    a = param_1->arrA[param_3];
    if (a[param_4] == 0) {
        a[param_4] = func_ov006_020d813c(-1, 4, 2);
        if (param_3 == 0 && t == 0x2c && param_1->w20 == 0) {
            func_ov006_020d76c8(a[param_4], 1);
        } else {
            func_ov006_020d76c8(a[param_4], 0);
        }
        func_02014de4(a[param_4], 0x5e000);
    }
    func_ov006_020d6f80(a[param_4], ((int *)tab[param_3])[t]);
    if (param_3 == 0 && bit == 0 && b[param_4] == 0) {
        b[param_4] = func_ov006_020d813c(-1, 5, 2);
        func_02014de4(b[param_4], 0x5e000);
    }
    if (param_4 != param_1->cur[param_3]) {
        func_ov006_020d76e0(a[param_4]);
        if (b[param_4] != 0) {
            func_ov006_020d6f80(b[param_4], 0x79);
        }
    } else {
        func_ov006_020d76f4(a[param_4]);
        if (b[param_4] != 0) {
            func_ov006_020d6f80(b[param_4], 0x78);
        }
    }
}
