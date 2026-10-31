extern void func_0207303c();
extern void func_020349b0();
extern void func_02008378();
extern void func_020083b0();
extern unsigned char *func_0202970c();
extern int func_02029708();
extern void func_0205eb78();

struct Flags02060a14 {
    unsigned char bit0 : 1;
    unsigned char dirty : 1;
};

void func_02060a14(unsigned char *param_1)
{
    unsigned char auStack_24[12];
    unsigned char auStack_30[12];
    unsigned char auStack_3c[12];
    unsigned int uVar3;
    int iVar2;
    unsigned char *puVar1;

    if (param_1[0x7e] == 1 || ((struct Flags02060a14 *)(param_1 + 0x7d))->dirty) {
        func_0207303c(*(unsigned short **)(param_1 + 0xd0), *(unsigned short **)(param_1 + 0xd8), param_1[0x7f] * 0xc);
        ((struct Flags02060a14 *)(param_1 + 0x7d))->dirty = 0;
    }
    else {
        for (uVar3 = 0; uVar3 < param_1[0x7f]; uVar3 = (uVar3 + 1) & 0xff) {
            iVar2 = uVar3 * 0xc;
            func_020349b0(auStack_30, *(unsigned char **)(param_1 + 0xd0) + iVar2, 0x266);
            func_020349b0(auStack_3c, *(unsigned char **)(param_1 + 0xd8) + iVar2, 0xd9a);
            func_02008378(auStack_24, auStack_30, auStack_3c);
            func_020083b0(*(unsigned char **)(param_1 + 0xd8) + iVar2, auStack_24);
        }
    }
    puVar1 = func_0202970c();
    iVar2 = func_02029708(puVar1);
    func_0205eb78(*(unsigned char **)(param_1 + 0xdc), param_1[0x7f], iVar2, *(unsigned char **)(param_1 + 0xd0), *(unsigned char **)(param_1 + 0xd8));
}
