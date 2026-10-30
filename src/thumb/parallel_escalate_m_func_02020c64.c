typedef struct {
    int first;
    int second;
} Pair;

extern char data_02084f10;
extern char data_020a2938;

void func_02020c64(char *param_1)
{
    *(int *)param_1 = (int)&data_02084f10;
    *(unsigned short *)(param_1 + 4) = 0;
    *(unsigned short *)(param_1 + 6) = 0;
    *(int *)(param_1 + 8) = 0;
    *(int *)(param_1 + 0xc) = 0;
    *(int *)(param_1 + 0x14) = 0;
    *(Pair *)(param_1 + 0x1c) = *(Pair *)&data_020a2938;
    *(Pair *)(param_1 + 0x24) = *(Pair *)&data_020a2938;
    *(int *)(param_1 + 0x2c) = 0;
    *(int *)(param_1 + 0x30) = 0;
    *(int *)(param_1 + 0x34) = 0;
}
