extern void func_0202a714(int object,int a,int b);
extern void func_0202a16c(int object,int a);
extern void func_0202a5dc(int object,int a);
extern void func_0202aab4(int object,unsigned int index);
extern void func_0202aa64(int object,unsigned int index);

void func_0202a210(int object)
{
    unsigned short index;
    unsigned char *src;
    unsigned char byte;

    func_0202a714(object,0,0);
    func_0202a16c(object,0);
    func_0202a5dc(object,0);
    index = 0;
    do {
        func_0202aab4(object,index);
        index = index + 1;
    } while (index < 0x20);
    index = 0;
    do {
        func_0202aa64(object,index);
        index = index + 1;
    } while (index < 2);
    *(int *)(object + 0x30) = 0;
    *(int *)(object + 0x34) = 0;
    src = (unsigned char *)(object + 0x25);
    byte = *src;
    object = object + 0x24;
    *(unsigned char *)object = byte;
}
