extern char data_020c9e48;
extern char data_020c9e88;
extern int func_02071960();
extern int func_02071dd0();
extern unsigned short func_02071e40();

void func_0203f21c(int param_1)

{
    unsigned short uVar1;

    uVar1 = func_02071e40();
    *(unsigned int *)(param_1 + 0x48) = (unsigned int)uVar1;
    if (uVar1 != 4) {
        func_02071dd0();
        func_02071960(4);
    }
    *(unsigned int *)(((int)&data_020c9e48) + 0x54) = ((unsigned int)0x00210015u);
    *(short *)(((int)&data_020c9e88) + 0x1e) = (short)((unsigned int)0x00004007u);
    return;
}
