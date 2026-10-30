extern int func_0206c8ec();
extern unsigned int func_0206cd6c();

unsigned int
func_0206cdd0(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;
    unsigned char auStack_38 [44];
    unsigned int uStack_c;

    uStack_c = param_4;
    iVar1 = func_0206c8ec(auStack_38,param_1,param_2);
    if (iVar1 != 0) {
        if (func_0206cd6c(auStack_38) != 0) {
            return 1;
        }
    }
    return 0;
}
