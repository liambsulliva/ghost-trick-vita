extern int func_02027518();
extern int func_02037130();

unsigned int func_02038b3c(int param_1)

{
    int iVar1;

    iVar1 = func_02037130(*(unsigned int *)(param_1 + 0x1c) & 0xff);
    if ((iVar1 == 0xf) && (iVar1 = func_02027518(((unsigned int)0x00001023u)), iVar1 == 0)) {
        return 0;
    }
    iVar1 = func_02037130(*(unsigned int *)(param_1 + 0x1c) & 0xff);
    if ((iVar1 == 0x16) && (iVar1 = func_02027518(((unsigned int)0x00001024u)), iVar1 == 0)) {
        return 0;
    }
    iVar1 = func_02037130(*(unsigned int *)(param_1 + 0x1c) & 0xff);
    if ((iVar1 == 3) && (iVar1 = func_02027518(((unsigned int)0x00001025u)), iVar1 == 0)) {
        return 0;
    }
    return 1;
}
