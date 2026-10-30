extern char data_020c9e48;
extern char data_020c9e88;

int *func_02065954(int *param_1)
{
    char *puVar1;
    char *pcVar2;

    if (param_1[1] == 0) {
        puVar1 = &data_020c9e48;
        *(int *)puVar1 = param_1[2];
        *(unsigned short *)(puVar1 + 0xe) = *(unsigned short *)((char *)param_1 + 0xc);
        *(unsigned short *)(puVar1 + 0x12) = *(unsigned short *)((char *)param_1 + 0xe);
        *(unsigned short *)(puVar1 + 0x16) = *(unsigned short *)((char *)param_1 + 0x10);
        *(unsigned short *)(puVar1 + 0x1a) = *(unsigned short *)((char *)param_1 + 0x12);
        *(unsigned short *)(puVar1 + 6) = 0x1608;
        return param_1;
    }
    if (param_1[1] == 1) {
        pcVar2 = &data_020c9e88;
        *(int *)(&data_020c9e48 + 0x54) = param_1[2];
        *(unsigned short *)(pcVar2 + 0x22) = *(unsigned short *)((char *)param_1 + 0xc);
        *(unsigned short *)(pcVar2 + 0x26) = *(unsigned short *)((char *)param_1 + 0xe);
        *(unsigned short *)(pcVar2 + 0x2a) = *(unsigned short *)((char *)param_1 + 0x10);
        *(unsigned short *)(pcVar2 + 0x2e) = *(unsigned short *)((char *)param_1 + 0x12);
    }
    return param_1;
}
