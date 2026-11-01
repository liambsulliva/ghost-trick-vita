typedef struct Section2c8f0 {
    int count;
    int items[1];
} Section2c8f0;

typedef struct Header2c8f0 {
    unsigned short id;
    short pad;
    int offset1;
    int offset2;
    Section2c8f0 first;
} Header2c8f0;

typedef struct Obj2c8f0 {
    char pad[0xc];
    unsigned short id;
    short pad2;
    int f10;
    void *header;
    char pad3[0x6c];
    int count0;
    int *items0;
    int *end0;
    int count1;
    int *items1;
    int *end1;
    int count2;
    int *items2;
    int *end2;
} Obj2c8f0;

void func_0202c8f0(Obj2c8f0 *obj, Header2c8f0 *header)
{
    char *base;
    obj->header = header;
    obj->id = header->id;
    base = (char *)header->first.items;
    obj->count0 = header->first.count;
    obj->items0 = (int *)base;
    obj->end0 = (int *)base + obj->count0;
    obj->count1 = ((Section2c8f0 *)((char *)&header->first + header->offset1))->count;
    obj->items1 = (int *)(base + header->offset1);
    obj->end1 = (int *)(base + header->offset1) + obj->count1;
    obj->count2 = ((Section2c8f0 *)((char *)&header->first + header->offset2))->count;
    obj->items2 = (int *)(base + header->offset2);
    obj->end2 = (int *)(base + header->offset2) + obj->count2;
}
