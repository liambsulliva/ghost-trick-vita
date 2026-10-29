void func_02056978(void *object);
extern int data_020aef54;

void *func_0205bfcc(void *object)
{
    func_02056978(object);
    *(int *)object = (int)&data_020aef54;
    return object;
}
