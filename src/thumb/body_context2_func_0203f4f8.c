struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;
extern char data_020ce204;
extern int func_02009900();
extern void func_0202f7e8();
extern int func_0203efa8();
extern int func_0203f53c();

void func_0203f4f8(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    if (*(int *)(param_1 + 0x4c) == 0) {
        iVar1 = func_0203efa8(((unsigned int)&data_020ce204));
        *(int *)(param_1 + 0x4c) = iVar1;
        if (iVar1 != 0) {
            func_0203f53c(iVar1,*(unsigned int *)(param_1 + 0x20));
            uVar2 = func_02009900();
            func_0202f7e8(uVar2,*(unsigned int *)(param_1 + 0x4c),10,0);
            data_020c9e48.flags = ((unsigned int)0x80001000u) | data_020c9e48.flags;
        }
    }
    return;
}
