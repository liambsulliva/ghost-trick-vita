extern int func_02008418();
extern int func_02010dc4();

void func_020273c4(unsigned int param_1,unsigned char *param_2,int param_3)

{
    if (*(unsigned short *)(param_3 + 4) == 0) {
        func_02010dc4(param_2);
        return;
    }
    func_02008418(param_2);
    return;
}
