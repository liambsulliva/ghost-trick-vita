extern int func_020229b4();
extern int func_0203b1e0();

void func_0203b294(int param_1)

{
    if (*(int *)(param_1 + 0x10) == 1) {
        func_0203b1e0(*(unsigned int *)(param_1 + 0x14));
        func_020229b4(0x100);
        return;
    }
    if (*(int *)(param_1 + 0x10) == 2) {
        func_0203b1e0(*(unsigned int *)(param_1 + 0x14));
        func_020229b4(0x200);
    }
    return;
}
