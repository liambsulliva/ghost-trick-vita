extern int func_02009900();
extern int func_02014de4();
extern int func_0202f7e8();
extern int func_020465d8();
extern int func_02071960();
extern int func_02071dd0();
extern int func_ov003_020d58e8();
extern unsigned char data_020c9e48[];
extern unsigned char data_020c9e88[];

struct Bits {
    unsigned int lo : 8;
    unsigned int hi : 8;
    unsigned int rest : 16;
};

union Field {
    unsigned int raw;
    struct Bits bits;
};

struct Layout {
    unsigned char pad0[8];
    union Field field;
    unsigned char pad1[0x20];
    unsigned char col;
    unsigned char row;
};

void func_ov003_020d57f4(unsigned char *param_1)
{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned char *rec;
    int width;
    struct Layout *p;

    p = (struct Layout *)param_1;
    p->col = (p->field.raw & 0xff00) >> 8;
    p->row = p->field.bits.lo - 1;
    if (p->row >= 0x13) {
        p->row = 0x12;
    }
    func_02071dd0();
    width = 0x180;
    func_02071960(width);
    *(unsigned int *)(data_020c9e48 + 0x54) = (*(unsigned int *)(data_020c9e48 + 0x54) & 0x3ffffdff) | (width << 4);
    *(unsigned short *)(data_020c9e88 + 0x1e) = 0xc3;
    func_ov003_020d58e8(param_1);
    uVar2 = 0;
    do {
        rec = param_1 + uVar2 * 4;
        uVar1 = func_020465d8(((unsigned int)0x020ce768u));
        *(unsigned int *)(rec + 0x20) = uVar1;
        func_02014de4(uVar1, (uVar2 * 0xb4 + 6) * 0x1000);
        uVar1 = func_02009900();
        func_0202f7e8(uVar1, *(unsigned int *)(rec + 0x20), 1, 0);
        uVar2 = uVar2 + 1;
    } while (uVar2 < 2);
}
