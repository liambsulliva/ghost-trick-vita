class Object0202f630_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2(int argument0,int argument1);
};

extern "C" {
extern char data_02085654;

void func_0202f630(int param_1)
{
    ((Object0202f630_0 *)(param_1 + 4))->slot2(0,(int)&data_02085654);
}
}
