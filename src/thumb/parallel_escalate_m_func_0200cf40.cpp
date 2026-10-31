class Object0200cf40_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int index);
};

class Object0200cf40_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual unsigned short *slot3(unsigned int argument0);
};

extern "C" {
extern Object0200cf40_1 *func_02007b70(void *object);
extern void *func_02007bd8(void);
extern void *func_02009c1c(void);
extern unsigned int func_0200cd20(int object, int index);
extern void func_0200d114(int *dst, unsigned short *src, unsigned short *data, unsigned int key, int arg);
extern void func_0202f1bc(void *object, unsigned short *data, int size, unsigned int key, int arg);

void func_0200cf40(int *param_1, int param_2, int *param_3, int param_4)
{
    int size;
    int key;
    unsigned short *data;

    if (param_2 != -1) {
        size = ((Object0200cf40_0 *)param_1)->slot2(param_2) * param_1[0x11];
        key = func_0200cd20((int)param_1, param_2);
        if (size == 0) {
            key = key - param_1[0x11];
            if (key <= 0) {
                return;
            }
            size = size + param_1[0x11];
        }
        data = func_02007b70(func_02007bd8())->slot3(key);
        if (data != 0) {
            func_0200d114(param_3, (unsigned short *)((char *)param_1 + 0x48 + (size & ~1)), data, key, param_4);
            func_0202f1bc(func_02009c1c(), data, size, key, param_1[0x10]);
        }
    }
}
}
