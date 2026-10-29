void func_0203b4f4(void *object);
extern int data_020a706c;

void *func_0203b544(void *object)
{
    func_0203b4f4(object);
    *(int *)object = (int)&data_020a706c;
    return object;
}
