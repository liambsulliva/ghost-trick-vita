typedef int code();
extern char data_020d2f04;
extern int func_02075f84();
extern void func_0207a19c();

void func_0207a140(int param_1,unsigned int param_2)
{
    int iVar1;

    if ((param_2 & 63) == 17) {
        if (((unsigned int *)&data_020d2f04)[3] != 0) {
            return;
        }
        iVar1 = 0;
        if (((code **)&data_020d2f04)[6] != (code *)0) {
            iVar1 = (((code **)&data_020d2f04)[6])();
        }
        if (iVar1 != 0) {
            func_0207a19c();
        }
        ((unsigned int *)&data_020d2f04)[3] = 1;
        return;
    }
    func_02075f84();
}
