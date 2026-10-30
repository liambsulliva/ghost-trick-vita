typedef struct {
    unsigned short a;
    unsigned short b;
} Pair0201df98;

typedef struct {
    unsigned char count;
    unsigned char pad;
    Pair0201df98 pairs[6];
    unsigned short tail;
} Rec0201df98;

typedef struct {
    char pad[0x38];
    Rec0201df98 recs[3];
} Obj0201df98;

int func_0201df98(Obj0201df98 *object, unsigned int index)
{
    unsigned short slot;

    if (index > 2) {
        return 0;
    }
    slot = object->recs[index].count + 1;
    object->recs[index].pairs[slot].a = 0xff;
    object->recs[index].pairs[slot].b = 0;
    return 1;
}
