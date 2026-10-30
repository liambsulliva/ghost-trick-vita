class Part0202ef90 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(char *name, int value);
};

extern "C" {
extern char data_020855bc;
extern char data_020855c8;
extern void *func_02031254(unsigned int size);
extern Part0202ef90 *func_0201ef58(void *object);
extern void *func_0201e580(void *object, Part0202ef90 *part);
extern void func_0201e978(void *object, int index, int value);

void func_0202ef90(void **param_1, int param_2)
{
    Part0202ef90 *first;
    Part0202ef90 *second;
    void *node;

    if (param_2 == 0) {
        first = (Part0202ef90 *)func_02031254(0x50);
        if (first != 0) {
            first = func_0201ef58(first);
        }
        first->slot4(&data_020855bc, 1);
        second = (Part0202ef90 *)func_02031254(0x50);
        if (second != 0) {
            second = func_0201ef58(second);
        }
        second->slot4(&data_020855c8, 1);
    }
    node = func_02031254(0xcc);
    if (node != 0) {
        node = func_0201e580(node, first);
    }
    param_1[0] = node;
    node = func_02031254(0xcc);
    if (node != 0) {
        node = func_0201e580(node, second);
    }
    param_1[1] = node;
    if (param_1[0] != 0) {
        func_0201e978(param_1[0], 0, 0xb5a);
        func_0201e978(param_1[0], 3, 0xfb);
    }
}
}
