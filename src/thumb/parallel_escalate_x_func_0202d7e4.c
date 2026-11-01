typedef struct Entry2d7e4 {
    int f0;
    int key;
    int f8;
    int body;
} Entry2d7e4;

typedef struct Obj2d7e4 {
    void **vtable;
    char pad[0xcc];
    unsigned int count;
    int *offsets;
    char *base;
} Obj2d7e4;

typedef void (*Handle2d7e4)(Obj2d7e4 *obj, void *body, int a, int b, int c, int d);

void func_0202d7e4(Obj2d7e4 *obj, int key, int a, int b, int c, int d)
{
    unsigned int i;
    for (i = 0; i < obj->count; i++) {
        Entry2d7e4 *entry = (Entry2d7e4 *)(obj->base + obj->offsets[i]);
        void *body = &entry->body;
        if (entry->key == key) {
            ((Handle2d7e4)obj->vtable[0x4c / 4])(obj, body, a, b, c, d);
            return;
        }
    }
}
