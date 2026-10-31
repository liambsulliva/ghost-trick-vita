struct Object {
    void *methods;
};

extern char data_020aeea8;
extern int *func_02056978(int *param_1);
extern void func_0200f70c(void *p);

struct Object *func_0205ca5c(struct Object *object)
{
    unsigned char i;

    func_02056978((int *)object);
    object->methods = &data_020aeea8;
    *(short *)((char *)object + 0x58) = -1;
    *(unsigned char *)((char *)object + 0x60) = 0;
    *(unsigned char *)((char *)object + 0x61) = 0;
    *(int *)((char *)object + 0x80) = 0;
    *(int *)((char *)object + 0x8c) = 0;
    *(int *)((char *)object + 0x94) = 0;
    *(int *)((char *)object + 0x98) = 0;
    *(int *)((char *)object + 0x9c) = 0;
    *(int *)((char *)object + 0xa0) = 0;
    *(int *)((char *)object + 0x5c) = 0;
    *(int *)((char *)object + 0x64) = 0;
    *(int *)((char *)object + 0x68) = 0;
    *(int *)((char *)object + 0x6c) = 0;
    *(int *)((char *)object + 0x70) = 0;
    *(int *)((char *)object + 0x74) = 0;
    *(int *)((char *)object + 0x78) = 0;
    *(int *)((char *)object + 0x7c) = 0;
    func_0200f70c((char *)object + 0xa4);
    func_0200f70c((char *)object + 0xb0);
    func_0200f70c((char *)object + 0xbc);
    func_0200f70c((char *)object + 0xd4);
    func_0200f70c((char *)object + 0xe4);
    func_0200f70c((char *)object + 0xf0);
    func_0200f70c((char *)object + 0xfc);
    func_0200f70c((char *)object + 0x114);
    for (i = 0; i < 2; i++) {
        *(unsigned int *)((char *)object + (i << 2) + 0x84) = 0;
    }
    return object;
}
