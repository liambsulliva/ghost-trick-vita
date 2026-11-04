struct Entry {
    unsigned char kind;
    unsigned char pad1;
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned char pad8[4];
    unsigned short *src;
};

void func_0202b2ac(struct Entry *obj, unsigned short *src)
{
    unsigned short i;
    unsigned char k = obj->kind;
    obj->src = src;
    if (k == 2) {
        obj->a = src[0];
        obj->b = src[1];
        for (i = 0; i < obj->b; i++) {
            obj->c = src[10];
            return;
        }
    } else if (k == 7) {
        obj->a = src[0];
    } else if (k == 0x27) {
        obj->a = src[1];
    }
}
