typedef struct Item5c790 {
    char data[0x44];
} Item5c790;

typedef struct Obj5c790 {
    char pad[0x48];
    unsigned int count;
    char pad2[8];
    Item5c790 *items;
} Obj5c790;

extern void func_02029028(Item5c790 *item);

void func_0205c790(Obj5c790 *object)
{
    unsigned int i;
    if (object->items != 0) {
        for (i = 0; i < object->count; i++) {
            func_02029028(&object->items[i]);
        }
    }
}
