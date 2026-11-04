typedef struct Obj02042820 {
    char pad0[4];
    int handle;
    char pad8[4];
    int preset;
    char pad10[0x28];
    unsigned char state;
    char pad39;
    unsigned char parts;
    char pad3b[5];
    int src;
    char pad44[4];
    unsigned int size;
} Obj02042820;

extern void *func_0200eb78(void);
extern int func_02022bc0(void *heap, unsigned int size, int flag);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *dma, int src, int dst, unsigned int size, int mode);

void func_02042820(Obj02042820 *obj, int src, unsigned int size, int mode, unsigned char parts, int high, int unused, int preset)
{
    int flag;
    int handle;
    int off;

    obj->src = src;
    obj->size = size;
    obj->parts = parts;
    obj->preset = preset;
    flag = 0;
    if (high != 0) {
        flag = 1;
    }
    if (obj->state == 0xff) {
        return;
    }
    if (obj->state == 0) {
        if (obj->preset == -1) {
            obj->handle = func_02022bc0(func_0200eb78(), size, flag);
        } else {
            obj->handle = obj->preset;
        }
    }
    handle = obj->handle;
    if (handle == -1) {
        return;
    }
    if (obj->parts != 0) {
        size = size / obj->parts;
        off = size * obj->state;
        func_0202f1bc(func_02009c1c(), src + off, handle + off, size, 3);
        obj->state++;
        if (obj->state >= obj->parts) {
            obj->state = 0xff;
        }
    } else {
        obj->state = 0xff;
        obj->state = 0xff;
        func_0202f1bc(func_02009c1c(), src, handle, size, 3);
    }
}
