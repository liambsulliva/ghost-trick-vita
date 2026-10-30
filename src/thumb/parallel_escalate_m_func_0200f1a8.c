extern int func_0200a2f8(int handle,int value);

int func_0200f1a8(char *object,int value)
{
    *(int *)(object + 0xdd4) = value;
    return func_0200a2f8(*(int *)(object + 0xee0),value);
}
