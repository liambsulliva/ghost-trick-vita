extern void func_0206c6d8(int *a,int b,int c,int d);
extern void func_0206c7ec(int *a);

void func_0201efe4(int param_1,int param_2,int param_3,int param_4)
{
    int *object;
    int value;

    switch (param_3) {
    case 0:
        object = (int *)(param_1 + 8);
        value = 0;
        break;
    case 2:
        object = (int *)(param_1 + 8);
        value = 2;
        break;
    case 1:
        object = (int *)(param_1 + 8);
        value = 1;
        break;
    default:
        goto end;
    }
    func_0206c6d8(object,param_2,value,param_4);
end:
    func_0206c7ec((int *)(param_1 + 8));
}
