struct func_02031fb8_obj
{
    unsigned char pad[0x109c];
    unsigned char flag : 1;
};

int func_02031fb8(int param_1)
{
    if (((struct func_02031fb8_obj *)param_1)->flag != 0) {
        return 1;
    }
    return 0;
}
