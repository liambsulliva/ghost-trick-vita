typedef struct {
    int w[4];
} Ent02009df4;

extern int func_02009df0(Ent02009df4 *e);
extern void func_02009e74(Ent02009df4 *e, int flag);
extern void func_02009e84(Ent02009df4 *e, int flag);
extern void func_02009e94(Ent02009df4 *e, unsigned char index);
extern void func_0200a6f0(Ent02009df4 *e, unsigned short value);
extern void func_02009e98(Ent02009df4 *e, signed char a, signed char b);

void func_02009df4(int param_1, unsigned int param_2, int param_3, int param_4)
{
    unsigned int i;
    Ent02009df4 *e;

    for (i = 0; i < 0x10; i++) {
        if (func_02009df0((Ent02009df4 *)(param_1 + i * 0x10)) == 0) {
            break;
        }
    }
    if (i < 0x10) {
        if ((param_2 & 0x8000) != 0) {
            e = (Ent02009df4 *)(param_1 + i * 0x10);
            func_02009e74(e, 1);
            func_02009e84(e, 1);
        } else {
            e = (Ent02009df4 *)(param_1 + i * 0x10);
            func_02009e74(e, 0);
            func_02009e84(e, 0);
        }
        func_02009e94(e, i);
        func_0200a6f0(e, param_2 & ~0x8000);
        func_02009e98(e, param_3, param_4);
    }
}
