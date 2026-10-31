struct Shared {
    int a;
    int b;
    void *c;
};

extern struct Shared data_020cdd0c;
extern void func_0202f64c(void *object);
extern void func_02031270(void *object);
extern void func_020381a0(void *object);

void func_02037ce0(unsigned int *object)
{
    unsigned int *child;

    child = (unsigned int *)object[0xe];
    if (child == 0) {
        goto skip;
    }
    if (child == 0) {
        goto clear;
    }
    func_020381a0(child);
    func_02031270(child);
clear:
    object[0xe] = 0;
skip:
    if (data_020cdd0c.c != 0) {
        func_02031270(data_020cdd0c.c);
        data_020cdd0c.c = 0;
    }
    func_0202f64c(object + 0xf);
    func_0202f64c(object + 0x10);
    func_0202f64c(object + 0x11);
    func_0202f64c(object + 0x12);
    func_0202f64c(object + 0x13);
}
