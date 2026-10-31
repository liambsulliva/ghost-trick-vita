struct Object {
    void *methods;
};

extern char data_020b03a0;
extern char data_020c9e48[];
extern char data_020c9e88[];
struct Object *func_0206448c(struct Object *object);

struct Object *func_020644bc(struct Object *object)
{
    func_0206448c(object);
    *(int *)((char *)object + 0x38) = 0;
    object->methods = &data_020b03a0;
    *(unsigned short *)((char *)object + 0x50) = 0;
    *(unsigned short *)((char *)object + 0x52) = 0;
    *(unsigned char *)((char *)object + 0x78) = 0;
    *(unsigned char *)((char *)object + 0x79) = 0;
    *(int *)((char *)object + 0x3c) = 0;
    *(int *)((char *)object + 0x40) = 0;
    *(int *)((char *)object + 0x44) = 0;
    *(int *)((char *)object + 0x48) = 0;
    *(int *)((char *)object + 0x4c) = 0;
    *(int *)((char *)object + 0x54) = 0;
    *(int *)((char *)object + 0x58) = 0;
    *(int *)((char *)object + 0x5c) = 0;
    *(int *)((char *)object + 0x60) = 0;
    *(int *)((char *)object + 0x64) = 0;
    *(int *)((char *)object + 0x68) = 0;
    *(int *)((char *)object + 0x6c) = 0;
    *(int *)((char *)object + 0x70) = 0;
    *(int *)((char *)object + 0x74) = 0;
    *(unsigned char *)((char *)object + 0x7a) = 0;
    *(int *)((char *)object + 0x48) = *(int *)data_020c9e48;
    *(unsigned short *)((char *)object + 0x50) = *(unsigned short *)(data_020c9e48 + 0x1a);
    *(int *)((char *)object + 0x4c) = *(int *)(data_020c9e48 + 0x54);
    *(unsigned short *)((char *)object + 0x52) = *(unsigned short *)(data_020c9e88 + 0x26);
    return object;
}
