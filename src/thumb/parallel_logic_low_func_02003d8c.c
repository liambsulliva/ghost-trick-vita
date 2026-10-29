extern unsigned int func_02002f8c(int param_1,int param_2,int param_3,int param_4);

unsigned int func_02003d8c(int param_1,int param_2,int param_3)
{
    unsigned int mask;
    int index;
    int bit;

    mask = func_02002f8c(0xffffffff,param_1,param_2,param_3);
    bit = 1;
    for (index = 0; index < 0x11; index = index + 1) {
        if (index == 0x10) {
            break;
        }
        if (((bit << index) & mask) != 0) {
            break;
        }
    }
    return index;
}
