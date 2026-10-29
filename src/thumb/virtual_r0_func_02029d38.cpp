class Object02029d38_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int argument0,int argument1);
};

extern "C" {
extern char data_02085220;
typedef int code(...);

unsigned int func_02029d38(int param_1,unsigned int param_2)

{


    unsigned int uVar1;


    uVar1 = ((Object02029d38_0 *)(*(int **)(param_1 + 4)))->slot2((int)(param_2),(int)(((unsigned int)&data_02085220)));
    return uVar1;
}
}
