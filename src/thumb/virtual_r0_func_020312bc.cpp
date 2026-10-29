class Object020312bc_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int argument0,int argument1);
};

extern "C" {
typedef int code(...);
extern int func_02007b70(...);
extern int func_02007bd8(...);

unsigned int func_020312bc(unsigned int param_1,unsigned int param_2)

{


    int *piVar1;
    unsigned int uVar2;


    func_02007bd8();
    piVar1 = (int *)func_02007b70();
    uVar2 = ((Object020312bc_0 *)(piVar1))->slot2((int)(param_1),(int)(param_2));
    return uVar2;
}
}
