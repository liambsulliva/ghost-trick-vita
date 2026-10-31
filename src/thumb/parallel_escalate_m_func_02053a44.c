struct Flags02053a44 {
    unsigned char a : 1;
    unsigned char b : 1;
};

extern char data_020cf8b4;
extern void func_0205964c();
extern void *func_02008444();
extern int func_0200f330();
extern int func_02008828();
extern int func_02053b08();
extern void *func_020083a0();
extern void func_0205958c();
extern void func_02008378();

void func_02053a44(int *param_1, int param_2, int param_3, int param_4)
{
    int iVar2;
    int *puVar5;
    unsigned char *puVar4;
    int aiStack_24[3];
    int auStack_30[3];

    func_0205964c(param_1, param_2, param_3, param_4);
    if (func_0200f330(func_02008444()) == 0) {
        iVar2 = 0;
        if (param_1[0x36] != 0 && func_02008828(param_1[0x36], &data_020cf8b4) != 0) {
            iVar2 = func_02053b08(param_1[0x36]);
        }
        puVar5 = (int *)param_1[0x36];
        if (puVar5 != 0 && iVar2 == 0) {
            puVar4 = func_020083a0(puVar5);
            func_0205958c(auStack_30, puVar5, *((unsigned char *)param_1 + 0xdc));
            func_02008378(aiStack_24, puVar4, auStack_30);
            param_1[0x2c] = aiStack_24[0] - *(int *)((char *)param_1 + *((unsigned char *)param_1 + 0xdd) * 0xc + 0xe4);
            param_1[0x2d] = aiStack_24[1] - *(int *)((char *)param_1 + *((unsigned char *)param_1 + 0xdd) * 0xc + 0xe8);
            if (!((struct Flags02053a44 *)((char *)param_1 + 0xde))->a &&
                ((struct Flags02053a44 *)((char *)param_1 + 0xde))->b) {
                ((struct Flags02053a44 *)((char *)param_1 + 0xde))->b = 0;
                param_1[0x36] = 0;
            }
        }
    }
}
