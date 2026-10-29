extern int func_ov006_020d7108();
extern int func_ov006_020d809c();
extern int func_ov006_020d813c();

void func_ov006_020d5788(int param_1,unsigned int param_2)

{
    unsigned int uVar1;

    uVar1 = func_ov006_020d809c(((int)0x0000800au),((int)0x0000800au) + 1,((int)0x0000800au) + 1,6,0xaa,0x3a,0xbc);
    *(unsigned int *)(param_1 + 0x410) = uVar1;
    uVar1 = func_ov006_020d809c(0x6c,0x6d,0x70,0xffffff80,0xfffffff4,0xffffff98,0xc);
    *(unsigned int *)(param_1 + 0x418) = uVar1;
    uVar1 = func_ov006_020d809c(0x6e,0x6f,0x71,0x68,0xfffffff4,0x80,0xc);
    *(unsigned int *)(param_1 + 0x41c) = uVar1;
    uVar1 = func_ov006_020d813c(((int)0x0000800au) + -5,6,2);
    *(unsigned int *)(param_1 + 0x42c) = uVar1;
    uVar1 = func_ov006_020d813c(((int)0x0000800au) + -4,6,2);
    *(unsigned int *)(param_1 + 0x430) = uVar1;
    func_ov006_020d7108(param_1,param_2,1);
    *(unsigned char *)(param_1 + 0x24) = 5;
    return;
}
