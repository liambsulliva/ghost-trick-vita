extern char data_020b18d8;
extern char data_020b1908;
extern int func_02006db0();
extern int func_02006ebc();
extern int func_02006ecc();
extern int func_020073dc();
extern int func_020078fc();

void func_02006e84(unsigned int param_1,int param_2)

{
    int *piVar1;
    unsigned int uVar2;

    piVar1 = ((int *)&data_020b1908);
    *((int *)&data_020b1908) = param_2;
    uVar2 = func_02006ecc();
    func_02006ebc(*piVar1,0,uVar2);
    func_02006db0(param_1);
    func_020073dc();
    func_020078fc();
    *((int *)&data_020b18d8) = *piVar1 + 0x20;
    return;
}
