typedef struct {
    int pad;
    signed char *base;
    int pad2;
    int size;
} Table02022350;

extern signed char data_02084f18;
extern int func_020790c8(signed char *a, signed char *b);
extern int func_02079020(signed char *a);

signed char *func_02022350(Table02022350 *table, signed char *name)
{
    signed char *entry;

    entry = table->base;
    if (name[0] == 0) {
        name = &data_02084f18;
    }
    for (; entry < table->base + table->size; entry += func_02079020(entry) + 1) {
        if (entry[0] == name[0] && func_020790c8(entry + 1, name + 1) == 0) {
            return entry;
        }
    }
    return 0;
}
