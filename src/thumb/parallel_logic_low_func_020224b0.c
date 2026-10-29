extern int func_020224cc(int object,int bits,int value);

int func_020224b0(int object,int value)
{
    if (value >= 0) {
        if (func_020224cc(object,object + 0x20,value) != 0) {
            return 1;
        }
    }
    return 0;
}
