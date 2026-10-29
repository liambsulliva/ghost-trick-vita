extern unsigned short data_02084848[];

void func_02017310(int param_1)
{
    short *cursor;
    int index;
    int step;

    cursor = ((short **)param_1)[5];
    if (cursor != 0) {
        index = (short)(*cursor + 0x100);
        if (index >= 0) {
            step = data_02084848[index] + 1;
            ((short **)param_1)[5] = cursor + step;
        }
    }
}
