class Object0204b46c_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int argument0, void *argument1);
};

extern "C" {
extern char data_020a971c;
extern char data_0209b874;
extern int func_02007b70(...);
extern int func_02007bd8(...);
extern int func_02047120(...);
extern void func_0200f70c(...);
extern void func_0204b460(...);

unsigned int *func_0204b46c(unsigned int *param_1)
{
    int *piVar1;
    int iVar2;

    func_02047120(param_1);
    *param_1 = (unsigned int)&data_020a971c;
    param_1[0x2e] = 0;
    func_0200f70c(param_1 + 0x2f);
    *(unsigned char *)(param_1 + 0x32) = 0;
    func_02007bd8();
    piVar1 = (int *)func_02007b70();
    iVar2 = ((Object0204b46c_1 *)piVar1)->slot2(0xcc, &data_0209b874);
    param_1[0x2e] = iVar2;
    func_0204b460(iVar2, 0xcc);
    return param_1;
}
}
