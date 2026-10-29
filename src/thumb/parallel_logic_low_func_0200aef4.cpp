class Object0200aef4_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2(int argument0,int argument1);
};

extern "C" {
extern char data_02083b24;

void func_0200aef4(int param_1)
{
    ((Object0200aef4_0 *)(param_1 + 0x1568))->slot2(0,(int)&data_02083b24);
}
}
