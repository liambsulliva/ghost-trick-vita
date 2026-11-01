typedef struct Rec02002a88 {
    int obj;
    int pad[3];
} Rec02002a88;

typedef struct Table02002a88 {
    int pad0;
    int pad4;
    unsigned int count;
    int padc;
    int pad10;
    Rec02002a88 recs[1];
} Table02002a88;

typedef struct Kinds02002a88 {
    char pad[0x10];
    int primary;
    int secondary;
} Kinds02002a88;

typedef struct Mgr02002a88 {
    char pad[0x90];
    Table02002a88 *table;
    int pad94;
    Kinds02002a88 *kinds;
} Mgr02002a88;

struct State {
    unsigned int before[4];
    int data;
};

typedef struct Entry02002a88 {
    unsigned int index;
    unsigned short ids[4];
} Entry02002a88;

typedef struct Node02002a88 {
    unsigned int index : 24;
    unsigned int flags : 8;
} Node02002a88;

extern struct State data_020b18a0;
extern void *func_02000d9c(Mgr02002a88 *mgr, unsigned int id, int kind);
extern void func_02002820(int owner, int obj, unsigned int slot);
extern void func_02007a54(int obj);
extern void func_02007a4c(int obj);

int func_02002a88(unsigned int param_1, unsigned int param_2)
{
    struct State *st;
    Mgr02002a88 *mgr;
    Entry02002a88 *entry;
    Node02002a88 *node;
    Table02002a88 *table;
    int owner;
    unsigned short i;

    st = &data_020b18a0;
    mgr = *(Mgr02002a88 **)(st->data + 0x1580);
    entry = (Entry02002a88 *)func_02000d9c(mgr, param_1, mgr->kinds->primary);
    table = (*(Mgr02002a88 **)(st->data + 0x1580))->table;
    if (entry->index >= table->count) {
        return -10;
    }
    if ((param_2 & 4) != 0 && (owner = table->recs[entry->index].obj) != 0) {
        for (i = 0; i < 4; i++) {
            if (entry->ids[i] != 0xffff) {
                mgr = *(Mgr02002a88 **)(st->data + 0x1580);
                node = (Node02002a88 *)func_02000d9c(mgr, entry->ids[i], mgr->kinds->secondary);
                if ((node->flags & 1) != 0) {
                    func_02002820(owner, (*(Mgr02002a88 **)(st->data + 0x1580))->table->recs[node->index].obj, i);
                } else {
                    func_02007a54((*(Mgr02002a88 **)(st->data + 0x1580))->table->recs[node->index].obj);
                    (*(Mgr02002a88 **)(st->data + 0x1580))->table->recs[node->index].obj = 0;
                }
            }
        }
    }
    if ((*(Mgr02002a88 **)(data_020b18a0.data + 0x1580))->table->recs[entry->index].obj != 0) {
        func_02007a4c((*(Mgr02002a88 **)(data_020b18a0.data + 0x1580))->table->recs[entry->index].obj);
    }
    (*(Mgr02002a88 **)(data_020b18a0.data + 0x1580))->table->recs[entry->index].obj = 0;
    return 0;
}
