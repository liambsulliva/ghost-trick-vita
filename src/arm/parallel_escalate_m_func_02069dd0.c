typedef struct Common {
    int *cmd;
    unsigned int flag;
    int priority;
    int f0c;
    int f10;
    int pad[5];
} Common;

extern Common data_020d02a0;

void func_02069dd0(void)
{
    char *p;

    p = (char *)&data_020d02a0;
    *(int *)(p + 0x14) = -3;
    *(int *)(p + 0x18) = 0;
    *(int *)(p + 0x24) = 0;
    *(int *)(p + 0x20) = 0;
    *(int *)(p + 0x1c) = 0;
}
