struct State {
    unsigned char before_busy[44];
    unsigned int busy;
    unsigned int fourth;
    unsigned int fifth;
    unsigned int third;
};
extern struct State data_020d2ec0;
extern int func_02075c98();
extern int func_02075cac();
extern void func_020786c8();

unsigned int
func_020781f4(int param_1,int param_2,unsigned int param_3,unsigned int param_4,unsigned int param_5)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02075c98();
    if (data_020d2ec0.busy != 0) {
        func_02075cac(uVar1);
        return 1;
    }
    data_020d2ec0.busy = 1;
    data_020d2ec0.third = param_3;
    data_020d2ec0.fourth = param_4;
    data_020d2ec0.fifth = param_5;
    iVar2 = 0;
    if (0 < param_2) {
        do {
            func_020786c8(*(unsigned int *)(param_1 + iVar2 * 4));
            iVar2 = iVar2 + 1;
        } while (iVar2 < param_2);
    }
    func_02075cac(uVar1);
    return 0;
}
