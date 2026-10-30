struct Strings020223b8 {
    int unk0;
    signed char *text;
    int size;
    int count;
};

int func_02022324(struct Strings020223b8 *table);

void func_020223b8(struct Strings020223b8 *table, signed char *text, int size)
{
    signed char *p;

    if (size == 0) {
        p = text;
        for (;;) {
            if (p[0] == 0 && p[1] == 0) {
                break;
            }
            p++;
            size++;
        }
        size += 2;
    }
    table->text = text;
    table->size = size;
    if (text != 0) {
        table->count = func_02022324(table) + 1;
    }
}
