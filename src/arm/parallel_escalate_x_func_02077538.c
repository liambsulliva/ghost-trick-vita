typedef struct Rec10_02077538 {
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned short d;
    unsigned short e;
} Rec10_02077538;

typedef struct Rec12_02077538 {
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned short d;
    unsigned short e;
    unsigned short f;
} Rec12_02077538;

typedef struct Small02077538 {
    unsigned char type;
    Rec10_02077538 rec;
} Small02077538;

typedef union Out02077538 {
    unsigned char type;
    Small02077538 small;
    Rec12_02077538 big;
} Out02077538;

typedef struct Range02077538 {
    unsigned char first;
    unsigned char last;
    Rec12_02077538 recs[1];
} Range02077538;

typedef struct Slots02077538 {
    unsigned char used[8];
    Rec12_02077538 recs[8];
} Slots02077538;

typedef struct Table02077538 {
    char pad0[0x38];
    unsigned int count;
    unsigned int entries[1];
} Table02077538;

typedef struct Iter02077538 {
    unsigned int index;
    unsigned int sub;
} Iter02077538;

int func_02077538(const Table02077538 *tbl, Out02077538 *out, Iter02077538 *it)
{
    unsigned int entry;
    unsigned int offset;
    const Range02077538 *range;
    const Slots02077538 *slots;

    while (it->index < tbl->count) {
        entry = tbl->entries[it->index];
        out->type = (unsigned char)entry;
        offset = entry >> 8;
        switch ((int)(entry & 0xff)) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            out->small.rec = *(const Rec10_02077538 *)((const char *)tbl + offset);
            it->index++;
            return 1;
        case 16:
            range = (const Range02077538 *)((const char *)tbl + offset);
            while (it->sub < range->last - range->first + 1) {
                out->big = range->recs[it->sub];
                it->sub++;
                return 1;
            }
            break;
        case 17:
            slots = (const Slots02077538 *)((const char *)tbl + offset);
            while (it->sub < 8) {
                if (slots->used[it->sub] == 0) {
                    break;
                }
                out->big = slots->recs[it->sub];
                it->sub++;
                return 1;
            }
            break;
        }
        it->index++;
        it->sub = 0;
    }
    return 0;
}
