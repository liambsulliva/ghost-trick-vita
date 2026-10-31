typedef struct {
    int w[45];
} Table45;

typedef struct {
    int w[23];
} Table23;

struct Obj7108 {
    unsigned char pad00[0x25];
    unsigned char flags;
    unsigned char pad26[0xc];
    signed char cur[2];
    unsigned char pad34[4];
    signed char list[2][0x32];
    int vals[2][50];
};

extern int func_0201003c();
extern int func_02015c04();
extern int func_02027d4c();
extern void func_02073a8c();
extern void func_ov006_020d7238();
extern const Table45 data_ov006_020d8d34;
extern const Table23 data_ov006_020d8cd8;
extern char data_ov006_020d8e28;
extern char data_ov006_020d8e34;
extern char data_ov006_020d8e44;
extern char data_020c9e48;
extern char data_020c9e88;

void func_ov006_020d7108(struct Obj7108 *param_1, int param_2, int param_3)
{
    unsigned char bit;
    int idx;
    signed char *row;
    int *list;
    int *vals;
    int a;
    int b;
    int flags;
    int *tables[2];
    Table45 t45;
    Table23 t23;
    char buf1[128];
    char buf2[128];
    char buf0[128];

    t45 = data_ov006_020d8d34;
    t23 = data_ov006_020d8cd8;
    tables[0] = t45.w;
    tables[1] = t23.w;
    bit = param_1->flags & 1;
    row = param_1->list[bit];
    idx = row[param_1->cur[bit]];
    if (param_3 != 0) {
        vals = param_1->vals[bit];
        list = tables[bit];
        func_02073a8c(buf1, &data_ov006_020d8e28, list[idx], vals[idx]);
        func_02073a8c(buf2, &data_ov006_020d8e34, list[idx], vals[idx]);
        a = func_02027d4c(buf1, 3);
        b = 0;
        if (bit) {
            b = func_02027d4c(buf2, 3);
        }
        if (bit) {
            func_ov006_020d7238(param_2, b, a);
        } else {
            func_ov006_020d7238(param_2, a, b);
        }
    } else {
        flags = *(int *)(&data_020c9e48 + 0x54);
        vals = param_1->vals[bit];
        func_02073a8c(buf0, &data_ov006_020d8e44, tables[bit][idx], vals[idx]);
        a = func_02027d4c(buf0, 3);
        func_02015c04(func_0201003c(), a, 0xf0, 0x90, 1);
        *(int *)(&data_020c9e48 + 0x54) = flags | 0x200;
        *(unsigned short *)(&data_020c9e88 + 0x26) = 0x1ec;
        *(unsigned short *)(&data_020c9e88 + 0x24) = 0x1f0;
    }
}
