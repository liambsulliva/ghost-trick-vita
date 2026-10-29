extern int func_0202f64c();
extern unsigned char * func_02057994();

void func_0203d028(int param_1,int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char *puVar1;

    func_0202f64c((unsigned char *)(param_1 + 0x34));
    puVar1 = func_02057994(param_3,0x6f,0x100000,param_4,1);
    *(unsigned char **)(param_1 + 0x34) = puVar1;
    if (param_2 != 0) {
        *(unsigned short *)(param_1 + 0x44) = 0x100;
        return;
    }
    *(unsigned short *)(param_1 + 0x44) = 0;
    return;
}
