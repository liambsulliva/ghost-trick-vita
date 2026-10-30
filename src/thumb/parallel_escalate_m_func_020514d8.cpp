class Slot020514d8 {
public:
    virtual void v0();
    virtual void v4();
};

static inline void release(Slot020514d8 *slot)
{
    if (slot != 0) {
        slot->v4();
    }
}

extern "C" void func_020514d8(char *self)
{
    unsigned int i;
    Slot020514d8 *slot;

    for (i = 0; i < 0x10; i++) {
        slot = *(Slot020514d8 **)(self + i * 4 + 0xb8);
        if (slot != 0) {
            release(slot);
            *(Slot020514d8 **)(self + i * 4 + 0xb8) = 0;
        }
    }
    *(int *)(self + 0xf8) = 0;
}
