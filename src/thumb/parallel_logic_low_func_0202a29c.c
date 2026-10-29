extern void func_0202a2c0(int object,int value);

void func_0202a29c(int object,int value)
{
    if (*(short *)(object + 0x26) == 0) {
        if (value == 0x23) {
            func_0202a2c0(object,1);
        }
    }
    else if (value != 0x23) {
        func_0202a2c0(object,0);
    }
}
