extern int func_02075f84();

void func_0207a118(unsigned int param_1,unsigned int param_2)

{
    if ((param_2 & 0x3f) == 1) {
        *((unsigned short *)0x020d2f00u) = 1;
        return;
    }
    func_02075f84();
    return;
}
