struct State {
    unsigned int first;
    unsigned int mode;
    unsigned int value0;
    unsigned int value1;
    int index;
    unsigned int count;
    unsigned int value2;
    unsigned int value3;
};
extern struct State data_020b18d8;
extern char data_020b18dc;
extern int func_0200743c();

void func_020072d8(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = ((int)&data_020b18d8);
    data_020b18d8.mode = 7;
    data_020b18d8.value0 = param_1;
    data_020b18d8.value1 = param_2;
    data_020b18d8.index = 0xffffffff;
    data_020b18d8.count = 0;
    data_020b18d8.value2 = param_3;
    uVar2 = ((unsigned int)&data_020b18dc);
    data_020b18d8.value3 = param_4;
    func_0200743c(uVar2);
    return;
}
