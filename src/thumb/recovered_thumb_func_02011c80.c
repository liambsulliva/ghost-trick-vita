extern int func_02027518();

unsigned int func_02011c80(int param_1)

{
    int iVar1;
    int iVar2;

    iVar1 = ((int)0x00001009u);
    iVar2 = func_02027518(((int)0x00001009u));
    if (iVar2 != 0) {
        return 0;
    }
    if (*(int *)(param_1 + iVar1 + 0x8f) == 0) {
        return 0;
    }
    return 1;
}
