extern int func_0200b518();
extern int func_020280a8();
extern int func_0203fdd8();

void func_020505fc(char *object)
{
    int flag;

    flag = 1;
    func_0200b518(object + 0x28, 1, 0x80000000, 0x80000000);
    func_020280a8(object + 0x28, 0);
    func_0203fdd8(object + 0x28, 0);
    if (*(int *)(object + 0xbc) == 8) {
        *(unsigned char *)(object + 0x13) = flag;
    }
}
