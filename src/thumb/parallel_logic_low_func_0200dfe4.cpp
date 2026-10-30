extern "C" void *func_02007bd8(void);
extern "C" int func_02007b70(void *object);
extern "C" void func_0200e020(char *object, int value);
struct Heap {
    virtual void slot0();
    virtual void slot1();
    virtual char *slot2(int size, void *table);
};
extern "C" void func_0200dfe4(int param_1, int param_2, int param_3, int param_4)
{
    unsigned int i;
    int *slot;
    Heap *heap;
    char *buffer;
    i = 0;
    do {
        slot = (int *)(param_1 + i * 4);
        if (slot[8] == 0) {
            heap = (Heap *)func_02007b70(func_02007bd8());
            buffer = heap->slot2(0x300, (void *)0x02083cc8);
            slot[8] = (int)buffer;
            func_0200e020(buffer, 0x300);
        }
        i = i + 1;
    } while (i < 2);
}
