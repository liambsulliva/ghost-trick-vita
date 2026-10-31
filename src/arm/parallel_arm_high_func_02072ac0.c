extern int func_01ff80f4();
typedef int code();
extern int func_02072cc4();
extern int func_02073508();

void func_02072ac0(int param_1,unsigned int param_2,int param_3,unsigned int param_4,unsigned char *param_5,
                                    unsigned int param_6,int param_7)

{
    if (param_4 == 0) {
        if (param_5 == 0) {
            return;
        }
        (*(code *)param_5)(param_6);
        return;
    }
    func_02072cc4(param_1);
    if (param_5 != 0) {
        func_02073508(param_1,param_5,param_6);
        if (param_7 != 0) {
            func_01ff80f4(param_1,param_3,param_2,param_4 >> 2 | 0xc5000000,0x10);
            return;
        }
        func_01ff80f4(param_1,param_3,param_2,param_4 >> 2 | 0x45000000,0x14);
        return;
    }
    if (param_7 != 0) {
        func_01ff80f4(param_1,param_3,param_2,param_4 >> 2 | 0x85000000,0x10);
        return;
    }
    func_01ff80f4(param_1,param_3,param_2,param_4 >> 2 | 0x5000000,0x14);
}
