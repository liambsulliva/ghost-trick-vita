extern int func_02008418();
extern int func_02010dc4();

void func_02010da4(int param_1,int param_2)

{
    if (*(int *)(param_1 + ((int)0x00000fb4u)) != 0) {
        if (param_2 != 0) {
            func_02008418();
            return;
        }
        func_02010dc4();
    }
    return;
}
