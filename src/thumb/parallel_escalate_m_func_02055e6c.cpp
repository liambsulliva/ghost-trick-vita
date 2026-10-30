class Obj02055e6c {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20(int arg);
    char pad04[0x24];
    unsigned char state : 2;
    unsigned char bit2 : 2;
    unsigned char flag : 1;
};

extern "C" void func_02055e6c(Obj02055e6c *param_1)
{
    if (param_1->flag) {
        param_1->state = 0;
        param_1->v20(0);
        return;
    }
    param_1->state = 2;
}
