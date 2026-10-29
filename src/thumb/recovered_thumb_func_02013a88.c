extern char data_020842d0;
extern char data_020842d8;
extern char data_020842e8;
extern char data_020842f4;
extern char data_02084300;
extern int func_02008444();
extern int func_02010384();
extern int func_0201192c();
extern int func_02013514();
extern int func_020790c8();

void func_02013a88(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    int iVar2;
    unsigned int uVar3;

    uVar1 = func_02013514(param_3);
    iVar2 = func_020790c8(uVar1,((unsigned int)&data_020842d0));
    if (iVar2 == 0) {
        uVar1 = func_02008444();
        uVar3 = 9;
    }
    else {
        iVar2 = func_020790c8(uVar1,((unsigned int)&data_020842d8));
        if (iVar2 == 0) {
            uVar1 = func_02008444();
            uVar3 = 10;
        }
        else {
            iVar2 = func_020790c8(uVar1,((unsigned int)&data_020842e8));
            if (iVar2 == 0) {
                uVar1 = func_02008444();
                uVar3 = 0xb;
            }
            else {
                iVar2 = func_020790c8(uVar1,((unsigned int)&data_020842f4));
                if (iVar2 == 0) {
                    uVar1 = func_02008444();
                    uVar3 = 0xc;
                }
                else {
                    iVar2 = func_020790c8(uVar1,((unsigned int)&data_02084300));
                    if (iVar2 != 0) goto code_r0x02013af6;
                    uVar1 = func_02008444();
                    uVar3 = 0xd;
                }
            }
        }
    }
    func_0201192c(uVar1,uVar3);
code_r0x02013af6:
    uVar1 = func_02008444();
    func_02010384(uVar1,1);
    return;
}
