struct State {
    unsigned int before[4];
    int data;
};

struct Info {
    unsigned short mask;
    unsigned short pad;
    unsigned short offs[16];
};

extern struct State data_020b18a0;
extern void func_0207c594(void *object, int size);
extern unsigned int *func_02000d9c();
extern void func_02003138(int base, int zero, struct Info *info);
extern int func_0200322c(int item, int base);

int func_02003444(unsigned int param_1)
{
    struct State *st;
    int iVar1;
    unsigned int *puVar2;
    int i;
    int base;
    int ret;
    struct Info info;

    func_0207c594(&info, 0x24);
    st = &data_020b18a0;
    iVar1 = *(int *)(st->data + 0x1580);
    puVar2 = func_02000d9c(iVar1, param_1, *(int *)(*(int *)(iVar1 + 0x98) + 8));
    if (puVar2 == 0) {
        return 0;
    }
    base = *(int *)(*(int *)(*(int *)(st->data + 0x1580) + 0x90) + *puVar2 * 0x10 + 0x14);
    if (base == 0) {
        return 0;
    }
    base = base + *(int *)(base + 0x18);
    func_02003138(base, 0, &info);
    for (i = 0; i < 16; i++) {
        if (info.mask & (1 << i)) {
            ret = func_0200322c(base + info.offs[i], base);
            if (ret == 0) {
                break;
            }
        }
    }
    return ret;
}
