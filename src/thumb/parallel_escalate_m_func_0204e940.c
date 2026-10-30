extern int func_020130fc();
extern int func_0201313c();
extern int func_0201d494();
extern int func_0204e9c8();
extern int func_0204eaf4();
extern int func_0204eb30();
extern int func_020570e4();

void func_0204e940(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    int iVar4;
    unsigned int uVar5;
    int iVar6;
    unsigned char auStack_24 [12];
    unsigned int uStack_18;

    if (*(unsigned char *)(param_1 + 0x10) == 2) {
        uVar5 = 0;
        iVar1 = ((int)0x00000126u) + -0x20;
        uStack_18 = param_4;
        do {
            iVar6 = param_1 + uVar5 * 2;
            if (*(short *)(iVar6 + 0xc6) >= 0) {
                iVar4 = param_1 + uVar5;
                func_0204e9c8(auStack_24,param_1,*(unsigned char *)(iVar4 + ((int)0x00000126u)),
                                          *(unsigned char *)(iVar4 + iVar1),(int)*(short *)(iVar6 + 0xc6));
                uVar2 = func_0204eaf4(param_1,*(unsigned char *)(iVar4 + ((int)0x00000126u)),
                                                            (int)*(short *)(iVar6 + 0xc6));
                uVar3 = func_0204eb30(param_1,*(unsigned char *)(iVar4 + ((int)0x00000126u)),
                                                            (int)*(short *)(iVar6 + 0xc6));
                func_0201d494(param_1,auStack_24);
                func_0201313c(param_1,uVar2);
                func_020130fc(param_1,uVar3,uVar3,0x1000);
                func_020570e4(param_1);
            }
            uVar5 = uVar5 + 1;
        } while (uVar5 < 0x20);
    }
    return;
}
