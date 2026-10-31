typedef struct {
    unsigned short x;
    unsigned short y;
} Point;
struct Entry {
    unsigned char pad[4];
    unsigned short value;
    unsigned short left;
    unsigned short top;
    unsigned short right;
    unsigned short bottom;
};
struct Menu {
    unsigned char pad[0x82];
    unsigned short selected;
    unsigned short count;
    unsigned char pad86[0x8e - 0x86];
    struct Entry entries[1];
};
extern const Point data_02084a80;
extern unsigned int func_0201b6d8(struct Entry *entry, unsigned int x, unsigned int y);
extern Point func_0201b4c4(struct Entry *entry);
extern void func_0201b6fc(Point *dst, Point *src);
extern int func_0201b594(struct Entry *entry);

Point func_0201b65c(struct Menu *menu, unsigned int x, unsigned int y)
{
    Point result = data_02084a80;
    unsigned int i;

    for (i = 0; i < menu->count; i++) {
        if (func_0201b6d8(&menu->entries[i], x, y) != 0) {
            Point pos = func_0201b4c4(&menu->entries[i]);
            func_0201b6fc(&result, &pos);
            menu->selected = func_0201b594(&menu->entries[i]);
            break;
        }
    }
    return result;
}
