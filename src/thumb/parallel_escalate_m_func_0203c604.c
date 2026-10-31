extern const char data_0209ab04[];
extern unsigned char * func_02057994();
extern int func_0203cd24();
extern int func_0203cd64();
extern int func_0203cdec();
extern int func_020521d4();
extern unsigned int * func_02008444();
extern int func_02010384();

void func_0203c604(int param_1,int param_2)
{
    unsigned int uVar1;

    uVar1 = 0x00000142u;
    *(unsigned char **)(param_1 + 0x28) = func_02057994(uVar1,0x6f,0,0x34000,1);
    if (param_2 != 0) {
        *(unsigned char **)(param_1 + 0x30) = func_02057994(0x165,0x6f,0,0xa000,1);
    }
    else {
        *(unsigned char **)(param_1 + 0x30) = func_02057994(0x147,0x6f,0,0x14000,1);
        *(unsigned char **)(param_1 + 0x2c) = func_02057994(uVar1 + 0x50,0x6f,0,0xfffb4000u,1);
        func_0203cd64(param_1,0,0);
    }
    func_0203cdec(param_1);
    func_0203cd24(param_1);
    if (param_2 != 0) {
        *(unsigned char *)(param_1 + 4) = 4;
    }
    else {
        *(unsigned char *)(param_1 + 4) = 0;
    }
    func_020521d4(data_0209ab04,0,6,0x100);
    func_02010384(func_02008444(),1);
    return;
}
