extern unsigned short func_0202aa58();
extern unsigned int func_02006f5c();
extern unsigned int func_0202adc0();
extern unsigned int func_0202ae1c();
extern unsigned int func_0202ae00();
extern void func_0202aab4();

struct Slot {
    unsigned char used:1;
};

static inline unsigned int align_block(unsigned int size)
{
    return (size + 0x3ff) & 0xfffffc00;
}

unsigned int func_0202ace0(unsigned int param_1, int param_2)
{
    unsigned short uVar1;
    unsigned int uVar3;
    unsigned int uVar5;
    unsigned int uVar6;
    unsigned int need;
    int iVar4;
    int t;

    uVar1 = func_0202aa58(param_1, param_2);
    uVar3 = (unsigned int)uVar1;
    uVar6 = 0;
    if (uVar3 == 0xffff) {
        return 0xffff;
    }
    for (uVar5 = 0; uVar5 < 0x20; uVar5 = (uVar5 + 1) & 0xffff) {
        iVar4 = uVar5 * 0xc;
        if (uVar3 == *(unsigned short *)(param_1 + iVar4 + 0x4a)) {
            *(unsigned char *)(param_1 + 0x48 + iVar4) = *(unsigned char *)(param_1 + 0x48 + iVar4) | 2;
            return uVar5;
        }
    }
    need = align_block(func_02006f5c(uVar3, 6) + 0x14) - 0x14;
    if (need > func_0202adc0(param_1, 2)) {
        return 0xffff;
    }
    if (*(unsigned short *)(param_1 + 0x2a) >= 0x20) {
        func_0202aab4(param_1, func_0202ae1c(param_1));
    }
    while (need > func_0202ae00(param_1, 2)) {
        func_0202aab4(param_1, func_0202ae1c(param_1));
    }
    for (uVar3 = 0; uVar3 <= 0x20; uVar3 = (uVar3 + 1) & 0xffff) {
        if (uVar3 >= 0x20) {
            return 0xffff;
        }
        if (((struct Slot *)(param_1 + uVar3 * 0xc + 0x48))->used == 0) {
            uVar6 = uVar3;
            break;
        }
    }
    t = *(unsigned char *)(param_1 + 0x48 + uVar6 * 0xc);
    *(unsigned char *)(param_1 + 0x48 + uVar6 * 0xc) = t & ~2;
    return uVar6;
}
