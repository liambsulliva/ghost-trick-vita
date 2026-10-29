extern int WaitByLoop();
extern int func_02073944();
extern int func_020739fc();
extern int func_02075c98();
extern int func_02075cac();

void func_02079d24(unsigned int param_1,unsigned int *param_2)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;

    uVar1 = ((unsigned int)0x02ffffe8u);
    while( 1 ) {
        uVar2 = func_02075c98();
        param_2[1] = uVar2;
        uVar2 = func_020739fc(uVar1);
        *param_2 = uVar2 & 0x40;
        if ((uVar2 & 0x40) != 0) {
            return;
        }
        iVar3 = func_02073944(param_1);
        if (iVar3 == 0) break;
        func_02075cac(param_2[1]);
        WaitByLoop(1);
    }
    return;
}
