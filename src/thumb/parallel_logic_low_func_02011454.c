struct Flags {
    unsigned int before : 13;
    unsigned int flag : 1;
};
extern int func_02027518();

void func_02011454(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02027518(((unsigned int)0x00001020u));
    ((struct Flags *)(param_1 + 8))->flag = uVar1;
    return;
}
