typedef struct Table63294 {
    unsigned short v[5];
} Table63294;

typedef struct Obj63294 {
    void **vtable;
    char pad[0x90];
    unsigned short h94;
} Obj63294;

typedef void (*Set63294)(Obj63294 *obj, int value);

extern Table63294 data_0209dd38;
extern Table63294 data_0209dd20;

void func_02063294(Obj63294 *obj, int index)
{
    Table63294 a;
    Table63294 b;
    if (index < 0) {
        index = 0;
    }
    if (index > 4) {
        index = 4;
    }
    a = data_0209dd38;
    b = data_0209dd20;
    obj->h94 = b.v[index];
    ((Set63294)obj->vtable[0x84 / 4])(obj, a.v[index]);
}
