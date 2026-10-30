struct Key {
    unsigned char first;
    unsigned char second;
    unsigned char third;
    unsigned char fourth;
};
extern unsigned char data_020cff60;
extern void func_0200958c();
extern void func_02009750(void *object);
extern void func_02009760(void *object);
extern void func_02009440(int kind,struct Key *key,int size,void (*callback)(),void *object);
extern int func_0206a8bc(void);
extern void func_020740d0(int value);
extern int func_02069b94(void);
int func_020095dc(void *object)
{
    struct Key key;
    int result;
    func_02009750(object);
    key.first = data_020cff60;
    key.second = key.third = key.fourth = 0;
    func_02009440(0x20,&key,4,func_0200958c,object);
    while (func_0206a8bc() == 0) {
        func_020740d0(0);
    }
    result = func_02069b94();
    func_02009760(object);
    return result;
}
