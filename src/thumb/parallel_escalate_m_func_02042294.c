extern char data_020a7e18;
extern char data_020ce5fc;
extern int *data_020ce5c4;
void func_0200ca28(void *object);
void func_020426b0(int param_1, int param_2);

int *func_02042294(int *object)
{
    int *current;

    func_0200ca28(object);
    *object = (int)&data_020a7e18;
    object[5] = 0;
    object[6] = 0;
    object[7] = 0;
    object[8] = 0;
    if (object == (int *)&data_020ce5fc) {
        return object;
    }
    current = data_020ce5c4;
    if (current == 0) {
        func_020426b0((int)object, (int)&data_020ce5fc);
    } else {
        func_020426b0((int)object, (int)current);
    }
    data_020ce5c4 = object;
    return object;
}
