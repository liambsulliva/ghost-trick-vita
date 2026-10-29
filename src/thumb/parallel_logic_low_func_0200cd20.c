typedef struct {
    unsigned int low : 24;
    unsigned int high : 8;
} LowWord24;

unsigned int func_0200cd20(int param_1,int param_2)
{
    LowWord24 *item;

    item = (LowWord24 *)(*(int *)(param_1 + 0x3c) + param_2 * 0x18 + 8);
    return item->low;
}
