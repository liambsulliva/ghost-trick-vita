extern char data_020b18a0;

#define WORK() (*(char **)((char *)&data_020b18a0 + 0x10))

int func_020018b0(void)
{
    char *work = WORK();
    int total = ((*(int *)(work + 0x15a8) + 0x1f) & ~0x1f) + ((*(int *)(work + 0x15b0) + 0x1f) & ~0x1f);

    total = total + ((*(int *)(work + 0x1b8c) * 8 + 0x1f) & ~0x1f);
    total = total + *(int *)(work + 0x1b88);
    return total + *(int *)(work + 0x1b90);
}
