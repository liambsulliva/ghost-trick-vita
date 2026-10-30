class Object02059ad0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
};

extern "C" {
void func_02059b18(int param_1, int param_2);
int func_02029ab4(int param_1);
void func_02008a98(int param_1);

void func_02059ad0(int param_1)
{
    *(unsigned char *)(param_1 + 0xde) = *(unsigned char *)(param_1 + 0xde) | 0x10;
    *(unsigned char *)(param_1 + 0xde) = *(unsigned char *)(param_1 + 0xde) & ~8;
    func_02059b18(param_1 + 0x114, 1);
    ((Object02059ad0 *)param_1)->slot11();
    if (func_02029ab4(param_1 + 0x114) != 0) {
        func_02008a98(param_1 + 0x114);
    }
}
}
