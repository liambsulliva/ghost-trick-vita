extern int func_020226e8();

void func_02022c18(int param_1,unsigned int param_2)

{
    if (param_2 != 0xffffffff) {
        if (param_2 < 0x6000) {
            func_020226e8(param_1 + ((int)0x00000f08u),param_2);
            return;
        }
        func_020226e8(param_1 + ((int)0x00000f3cu),param_2 - 0x6000);
    }
    return;
}
