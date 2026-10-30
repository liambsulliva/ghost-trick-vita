class Object {
public:
    virtual void slot0();
    virtual void slot1();
};

extern "C" void func_020590c0(char *param_1)
{
    unsigned int i;
    Object *child;

    for (i = 0; i < *(unsigned char *)(param_1 + 0xbe); i++) {
        child = *(Object **)(param_1 + i * 4 + 0xc4);
        if (child != 0) {
            child->slot1();
        }
        *(Object **)(param_1 + i * 4 + 0xc4) = 0;
    }
    *(unsigned char *)(param_1 + 0xbe) = 0;
}
