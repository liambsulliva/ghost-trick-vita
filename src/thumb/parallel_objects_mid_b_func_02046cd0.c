struct ScriptObject {
    unsigned char pad0[0x10];
    unsigned char flag10;
    unsigned char pad11[0x17];
    int f28;
    int f2c;
    unsigned short f30;
};

void func_02046cd0(struct ScriptObject *param_1)
{
    param_1->f2c = -1;
    param_1->f28 = 0;
    param_1->flag10 = 0;
    if (param_1->f30 == 0) {
        param_1->f30 = 1;
    }
}
