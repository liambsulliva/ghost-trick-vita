struct Slot0201a148 {
    int handle;
    unsigned short value;
};

struct Object0201a148 {
    char before[0x3a];
    unsigned char index;
    char middle[0xe0 - 0x3b];
    struct Slot0201a148 slots[3];
};

extern char data_020b9df4[];

extern int func_0201a1b0(void *object);
extern void func_0201708c(struct Object0201a148 *object);
extern void func_0201aa78(struct Object0201a148 *object);
extern void func_0201c634(void *object, int handle, int a, int b, int c);
extern void func_0201a108(void *object);
extern void func_0201aa4c(struct Object0201a148 *object);

int func_0201a148(struct Object0201a148 *object)
{
    char *base;

    if (object->index < 3) {
        object->slots[object->index].value = func_0201a1b0(data_020b9df4);
    }
    func_0201708c(object);
    func_0201aa78(object);
    if (object->index < 3) {
        base = data_020b9df4;
        func_0201c634(base, object->slots[object->index].handle, 0x100, 0x14, 2);
        func_0201a108(base);
        func_0201aa4c(object);
    } else {
        return 1;
    }
    return 0;
}
