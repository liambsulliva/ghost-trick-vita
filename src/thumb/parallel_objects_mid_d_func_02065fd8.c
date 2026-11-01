int func_02065fd8(int param_1)
{
    int object = *(int *)(param_1 + 0xc8);
    if (object != 0) {
        return (*(int (**)(int))(*(int *)object + 0x14))(object);
    }
    return 0;
}
