int func_0201f110(int param_1,int param_2,int param_3,int param_4)
{
    int diff;

    if (param_3 != (int)0x80000000) {
        return param_3;
    }
    diff = param_2 - param_1;
    if (diff >= 0) {
        return diff / param_4;
    }
    return -(-diff / param_4);
}
