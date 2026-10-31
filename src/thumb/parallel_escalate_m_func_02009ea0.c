typedef struct {
    int w[4];
} Ent02009ea0;

extern int func_02009df0(Ent02009ea0 *e);
extern void func_02009e98(Ent02009ea0 *e, int a, int b);
extern void func_0200a8c8(Ent02009ea0 *e, int a, int b);

void func_02009ea0(int param_1, int param_2, int param_3, int param_4, unsigned char param_5)
{
    unsigned int i;

    for (i = 0; i < 0x10; i++) {
        if (func_02009df0((Ent02009ea0 *)(param_1 + i * 0x10)) == 0) {
            break;
        }
    }
    if (i < 0x10) {
        func_02009e98((Ent02009ea0 *)(param_1 + i * 0x10), (signed char)(param_2 << 4), (signed char)(param_3 << 4));
        func_0200a8c8((Ent02009ea0 *)(param_1 + i * 0x10), (unsigned short)(param_4 << 4), (unsigned short)(param_5 << 4));
    }
}
