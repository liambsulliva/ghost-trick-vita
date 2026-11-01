typedef void (*Method63dbc)(void *self, int arg);

typedef struct Obj63dbc {
    void **vtable;
    char pad[0xc3];
    unsigned char state;
} Obj63dbc;

void func_02063dbc(Obj63dbc *object, unsigned char state)
{
    if (object->state != state) {
        object->state = state;
        if (object->state != 0) {
            ((Method63dbc)object->vtable[0x84 / 4])(object, 0);
        } else {
            ((Method63dbc)object->vtable[0x84 / 4])(object, 0x610);
        }
    }
}
