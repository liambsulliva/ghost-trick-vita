typedef struct {
    unsigned char used : 1;
    unsigned char pad;
    unsigned short id;
    unsigned int time;
    int x8;
} Slot;

typedef struct {
    unsigned char used : 1;
    unsigned char pad[7];
    int key;
} Request;

typedef struct {
    char head[0x48];
    Slot slots[32];
    char gap[0x1c];
    Request requests[8];
} Table;

extern int func_0202af00(Table *table, int key);
extern unsigned short func_0202aa58(Table *table, int key);

unsigned short func_0202ae1c(Table *table)
{
    unsigned short found;
    unsigned int best;
    unsigned int mask;
    int j;
    Request *req;
    Slot *slot;
    unsigned short id;
    unsigned int time;
    int i;

    found = 0;
    best = 0xffffffff;
    mask = 0;
    for (i = 0; i < 8; i++) {
        req = &table->requests[i];
        if (req->used) {
            if (func_0202af00(table, req->key) != 0) {
                id = func_0202aa58(table, req->key);
                for (j = 1; j < 0x20; j++) {
                    slot = &table->slots[j];
                    if (slot->used && id == slot->id) {
                        mask |= 1 << j;
                    }
                }
            }
        }
    }
    for (j = 1; j < 0x20; j++) {
        slot = &table->slots[j];
        if (slot->used && best > (time = slot->time) && ((1 << j) & mask) == 0) {
            found = j;
            best = time;
        }
    }
    if (found == 0) {
        for (j = 1; j < 0x20; j++) {
            slot = &table->slots[j];
            if (slot->used) {
                time = slot->time;
                if (best > time) {
                    found = j;
                    best = time;
                }
            }
        }
    }
    return found;
}
