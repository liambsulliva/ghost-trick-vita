extern char data_020b02b0;
extern char data_020c9e48;
extern char data_020c9e88;

void func_02065858(int *param_1,int param_2)
{
    char *puVar2;
    char *pcVar3;

    param_1[1] = param_2;
    *param_1 = (int)&data_020b02b0;
    param_1[2] = 0;
    *(unsigned short *)(param_1 + 3) = 0;
    *(unsigned short *)((char *)param_1 + 0xe) = 0;
    *(unsigned short *)(param_1 + 4) = 0;
    *(unsigned short *)((char *)param_1 + 0x12) = 0;
    if (param_2 == 0) {
        puVar2 = &data_020c9e48;
        param_1[2] = *(int *)puVar2;
        *(unsigned short *)(param_1 + 3) = *(unsigned short *)(puVar2 + 0xe);
        *(unsigned short *)((char *)param_1 + 0xe) = *(unsigned short *)(puVar2 + 0x12);
        *(unsigned short *)(param_1 + 4) = *(unsigned short *)(puVar2 + 0x16);
        *(unsigned short *)((char *)param_1 + 0x12) = *(unsigned short *)(puVar2 + 0x1a);
        return;
    }
    if (param_2 == 1) {
        pcVar3 = &data_020c9e88;
        param_1[2] = *(int *)(&data_020c9e48 + 0x54);
        *(unsigned short *)(param_1 + 3) = *(unsigned short *)(pcVar3 + 0x22);
        *(unsigned short *)((char *)param_1 + 0xe) = *(unsigned short *)(pcVar3 + 0x26);
        *(unsigned short *)(param_1 + 4) = *(unsigned short *)(pcVar3 + 0x2a);
        *(unsigned short *)((char *)param_1 + 0x12) = *(unsigned short *)(pcVar3 + 0x2e);
    }
    return;
}
