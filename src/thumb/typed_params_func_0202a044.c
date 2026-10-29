extern int func_0202a450();
extern int func_0202a6d8();

void func_0202a044(unsigned char *param_1,unsigned char *param_2,int param_3,unsigned int param_4)

{
    int iVar1;

    if ((unsigned char)param_1[0x24] != 2) {
        iVar1 = func_0202a450(param_1,param_2,(unsigned int)(unsigned char)param_1[0x24],param_4);
        func_0202a6d8((int)param_1,iVar1,param_3,param_4);
    }
    return;
}
