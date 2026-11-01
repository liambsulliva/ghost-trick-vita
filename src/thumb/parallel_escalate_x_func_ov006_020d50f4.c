typedef struct Obj20d50f4 {
    char *vtable;
    int value;
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char d;
} Obj20d50f4;

extern char data_ov006_020db160[];

void func_ov006_020d50f4(Obj20d50f4 *dest, const Obj20d50f4 *src)
{
    dest->vtable = data_ov006_020db160;
    dest->value = src->value;
    dest->a = src->a;
    dest->b = src->b;
    dest->c = src->c;
    dest->d = src->d;
}
