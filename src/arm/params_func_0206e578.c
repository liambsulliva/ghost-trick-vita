extern int func_0206c7f8();
extern int func_0206e3d4();

void func_0206e578(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int *piVar1;

    piVar1 = ((int *)0x020d0e40u);
    if (*((int *)0x020d0e40u) == 0) {
        *((int *)0x020d0e40u) = 1;
        func_0206e3d4(param_1,piVar1,1,param_4);
        func_0206c7f8();
        return;
    }
    return;
}
