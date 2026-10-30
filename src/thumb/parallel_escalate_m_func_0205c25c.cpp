class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int, int *);
};

extern "C" {
extern int data_020af480;
extern int data_0209dad4;
extern void func_02056ac8(...);
extern void func_02042718(...);
extern int func_02007bd8(...);
extern Object *func_02007b70(...);
}

extern "C" char *func_0205c25c(char *param_1)
{
    func_02056ac8(param_1);
    *(int *)param_1 = (int)&data_020af480;
    *(int *)(param_1 + 0xc8) = 0;
    *(int *)(param_1 + 0xcc) = 0;
    func_02042718(param_1 + 0xd0);
    *(int *)(param_1 + 0xc8) = func_02007b70(func_02007bd8())->slot2(0x800,&data_0209dad4);
    *(int *)(param_1 + 0xcc) = func_02007b70(func_02007bd8())->slot2(0x20,&data_0209dad4);
    return param_1;
}
