extern int func_020083b0();
extern int func_0201f138();
extern int func_0205d7b0();
extern int func_020611a8();

void func_0205d490(int param_1,unsigned char *param_2,unsigned int *param_3,unsigned int *param_4)

{
    int iVar1;

    *(unsigned int *)(param_1 + 0x8c) = 1;
    func_020083b0((unsigned int *)(param_1 + 0xb0),param_3);
    func_020083b0((unsigned int *)(param_1 + 0xfc),param_4);
    func_0205d7b0(param_1,(unsigned char *)(param_1 + 0xbc),(unsigned int *)(param_1 + 200),
                                (unsigned short *)(param_1 + 0xe0),(int *)(param_1 + 0xcc),(int *)(param_1 + 0xd0),
                                (unsigned char *)(param_1 + 0xa4),(unsigned char *)(param_1 + 0xb0));
    func_0205d7b0(param_1,(unsigned char *)(param_1 + 0xf0),(unsigned int *)(param_1 + 0x108),
                                (unsigned short *)(param_1 + 0x120),(int *)(param_1 + 0x10c),(int *)(param_1 + 0x110),
                                (unsigned char *)(param_1 + 0xe4),(unsigned char *)(param_1 + 0xfc));
    iVar1 = func_0201f138(0,0x8000,0x1800,0x18);
    *(int *)(param_1 + 0x90) = iVar1;
    func_020611a8(param_2,param_1 + 0xd4,param_1 + 0x114);
    return;
}
