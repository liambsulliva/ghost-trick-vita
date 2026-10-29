extern int func_0202fc08(int object,int value);

void func_0202fbf8(int object,int value)
{
    *(int *)(object + 8) = value;
    *(int *)(object + 4) = 0;
    func_0202fc08(object,0);
}
