struct Entry0202f1ec {
    int a;
    int b;
    int c;
    unsigned int kind;
};

extern void *func_0202f25c(void);
extern unsigned int func_0202f24c(void *object);
extern void *func_02007bd8(void);
extern void func_0201f2cc(void *object);
extern void func_0202f364(void *object, struct Entry0202f1ec *entry);

void func_0202f1ec(unsigned int *object)
{
    unsigned int i;
    struct Entry0202f1ec *entry;

    i = 0;
    *(unsigned int *)((int)object + 0x4024) = i;
    *(unsigned char *)((int)object + 0x4028) = i;
    if (func_0202f24c(func_0202f25c()) != 0) {
        func_0201f2cc(func_02007bd8());
        *object = i;
        *((unsigned char *)object + 0x4028) = 1;
        return;
    }
    entry = (struct Entry0202f1ec *)(object + 1);
    if (i < *object) {
        for (; i < *object; i++, entry++) {
            switch (entry->kind) {
            case 1:
            case 2:
            case 3:
                func_0202f364(object, entry);
                entry->kind = 0;
                break;
            }
        }
    }
}
