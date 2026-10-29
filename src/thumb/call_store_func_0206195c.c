void func_0202f524(void *object);
extern int data_020af698;

void *func_0206195c(void *object)
{
    func_0202f524(object);
    *(int *)object = (int)&data_020af698;
    return object;
}
