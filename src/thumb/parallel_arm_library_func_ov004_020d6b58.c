unsigned short func_ov004_020d61f0(void);
unsigned short func_ov004_020d6200(unsigned short);
int func_ov004_020d6b80(int, int);
int func_0202f25c(void);

int func_ov004_020d6b58(int param_1)

{
    int iVar1;
    unsigned short uVar2;
    unsigned short uVar3;

    uVar2 = func_ov004_020d61f0();
    uVar3 = 1;
    if (uVar2 == 0) {
        uVar3 = 0;
    }
    iVar1 = func_0202f25c();
    iVar1 = func_ov004_020d6b80(iVar1, param_1);
    func_ov004_020d6200(uVar3);
    return iVar1;
}
