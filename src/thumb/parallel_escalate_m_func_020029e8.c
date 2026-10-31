struct State {
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern unsigned int *func_02000d9c();
extern unsigned int func_02000d64();
extern unsigned int func_02002a88();

unsigned int func_020029e8(unsigned int param_1, unsigned int param_2)
{
    struct State *st;
    int iVar2;
    unsigned int *puVar3;
    unsigned int uVar4;
    unsigned int uVar5;
    unsigned int hdr[8];
    unsigned short ent[6];

    st = &data_020b18a0;
    iVar2 = *(int *)(st->data + 0x1580);
    puVar3 = func_02000d9c(iVar2, param_1, *(int *)(*(int *)(iVar2 + 0x98) + 0xc));
    if (puVar3 == 0) {
        uVar4 = 0xfffffffb;
    } else {
        if ((param_2 & 2) != 0) {
            uVar5 = 0;
            uVar4 = func_02000d64(*(int *)(st->data + 0x1580), *puVar3, hdr, 0x20, uVar5);
            if (uVar4 + 3 <= 1) {
                return uVar4;
            }
            for (; uVar5 < hdr[7]; uVar5 = uVar5 + 1) {
                uVar4 = func_02000d64(*(int *)(st->data + 0x1580), *puVar3, ent, 0xc, uVar5 * 0xc + 0x20);
                if (uVar4 + 3 <= 1) {
                    return uVar4;
                }
                uVar4 = func_02002a88(ent[2], 4);
                if (uVar4 + 3 <= 1) {
                    return uVar4;
                }
            }
        }
        uVar4 = 0;
        *(unsigned int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + *puVar3 * 0x10 + 0x14) = 0;
    }
    return uVar4;
}
