typedef int code();

extern unsigned int data_020c9d50[];
extern unsigned int data_020cff60[];

unsigned int func_02018dd4(int *param_1)
{
    unsigned int uVar1;

    uVar1 = data_020c9d50[1];
    if ((*(code **)(*param_1 + 0x3c))(param_1) != 0) {
        uVar1 = uVar1 | data_020c9d50[3];
        if ((data_020c9d50[0] & 0x200000) != 0 && (data_020cff60[2] & 3) == 0) {
            uVar1 = uVar1 | 0x1000;
        }
    }
    return uVar1;
}
