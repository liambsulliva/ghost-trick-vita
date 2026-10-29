typedef int code();

void func_0202a3a4(int object,int argument0)
{
    int *inner;

    inner = *(int **)(object + 4);
    (**(code **)(*inner + 0x10))(inner,argument0);
}
