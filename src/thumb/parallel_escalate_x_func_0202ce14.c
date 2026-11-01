typedef struct Section2ce14 {
    int count;
    int items[1];
} Section2ce14;

typedef struct Header2ce14 {
    unsigned short kind;
    unsigned short id;
    int offset1;
    int offset2;
    Section2ce14 first;
} Header2ce14;

typedef struct Obj2ce14 {
    char pad[0xc];
    unsigned short id;
    short pad2;
    int f10;
    void *header;
    char pad3[0x6c];
    unsigned short kind;
    char pad4[0x32];
    int count0;
    int *items0;
    int *end0;
    int count1;
    int *items1;
    int *end1;
    int count2;
    int *items2;
    int *end2;
} Obj2ce14;

void func_0202ce14(Obj2ce14 *obj, Header2ce14 *header)
{
    char *base;
    obj->header = header;
    obj->kind = header->kind;
    obj->id = header->id;
    base = (char *)header->first.items;
    obj->count0 = header->first.count;
    obj->items0 = (int *)base;
    obj->end0 = (int *)base + obj->count0;
    obj->count1 = ((Section2ce14 *)((char *)&header->first + header->offset1))->count;
    obj->items1 = (int *)(base + header->offset1);
    obj->end1 = (int *)(base + header->offset1) + obj->count1;
    obj->count2 = ((Section2ce14 *)((char *)&header->first + header->offset2))->count;
    obj->items2 = (int *)(base + header->offset2);
    obj->end2 = (int *)(base + header->offset2) + obj->count2;
}
