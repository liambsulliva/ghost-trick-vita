typedef int code();

extern unsigned int data_020c9d50;

int func_02017d00(int param_1)
{
    if ((*(code **)(*(int *)param_1 + 0x3c))(param_1) != 0) {
        if ((data_020c9d50 & 0x200000) != 0) {
            return 1;
        }
    }
    return 0;
}
