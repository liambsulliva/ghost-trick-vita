extern void func_0205bad0();
extern void func_0205bb40();

typedef void code();

struct ObjB {
    char pad0[0x10];
    unsigned char f10;
    char pad1[0x28 - 0x11];
    int *f28;
    char pad2[0x30 - 0x2c];
    int f30;
    char pad3[0x94 - 0x34];
    unsigned int f94;
    char pad4[0x9f - 0x98];
    unsigned char f9f;
};

void func_0205b960(struct ObjB *param_1)
{
    int *obj;

    func_0205bad0(param_1, 0x29);
    if (param_1->f30 != 0) {
        obj = param_1->f28;
        (**(code **)(*obj + 0x28))(obj);
        func_0205bb40(param_1);
    }
    param_1->f10 = 1;
    param_1->f94 = 0x266;
    param_1->f9f = 1;
}
