class Heap0202d748 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int size, char *name);
};

struct Table0202d748 {
    char pad[0xdc];
    unsigned int count;
    int entries;
};

extern "C" {
extern char data_02085298;
extern void *func_02007bd8(void);
extern Heap0202d748 *func_02007b70(void *object);
extern unsigned int func_0202d684(struct Table0202d748 *object, unsigned int index);
extern unsigned int func_0202d688(struct Table0202d748 *object, unsigned int index);
extern void func_0202d700(struct Table0202d748 *object, unsigned int index, unsigned int value);
extern void func_0202d710(struct Table0202d748 *object, unsigned int index, unsigned int value);

void func_0202d748(struct Table0202d748 *object, unsigned int count)
{
    unsigned int index;
    unsigned int first;
    unsigned int second;

    object->count = count;
    object->entries = func_02007b70(func_02007bd8())->slot2(object->count << 3, &data_02085298);
    for (index = 0; index < object->count; index++) {
        first = func_0202d684(object, index);
        second = func_0202d688(object, index);
        func_0202d700(object, index, first);
        func_0202d710(object, index, second);
    }
}
}
