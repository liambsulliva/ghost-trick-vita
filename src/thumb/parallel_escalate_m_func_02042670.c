struct List {
    char pad[0x14];
    unsigned int count;
    char *items;
};

extern void func_02042b20(void *entry);
extern void func_0207dea4(void *array, int size, int count, void *dtor);
extern void *func_0202dbdc(char *object);
extern void func_02023d1c(struct List *object);

void func_02042670(struct List *object)
{
    unsigned int i;

    for (i = 0; i < object->count; i++) {
        func_02042b20(object->items + i * 0x6c + 0x18);
    }
    if (object->items != 0) {
        func_0207dea4(object->items, 0x6c, 8, (void *)func_0202dbdc);
        object->items = 0;
    }
    func_02023d1c(object);
}
