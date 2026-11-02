typedef struct Entry4880 {
    short first;
    short pad2;
    short next;
    char pad6[4];
} Entry4880;

typedef struct Table4880 {
    char pad[0xc];
    Entry4880 e[1];
} Table4880;

typedef struct Menu4880 {
    char pad[0x158];
    Table4880 *table;
    char pad15c[4];
    signed char sel;
    char pad161;
    unsigned char busy;
    char pad163;
    signed char kind;
    char pad165[0xf];
    void **items;
} Menu4880;

extern void func_ov006_020d4968(Menu4880 *m, int a, int b);
extern void func_ov006_020d4944(void *item, int sel, int a, int b);
extern void func_0205964c(Menu4880 *m);

void func_ov006_020d4880(Menu4880 *m)
{
    signed char idx;
    Table4880 *t;
    short v;
    unsigned char i;

    idx = m->sel;
    t = m->table;
    v = t->e[idx].next;
    if (v != -2) {
        if (idx < 3 && v != -1 && t->e[idx + 1].first != -1) {
            m->sel = idx + 1;
        } else {
            m->sel = 0;
        }
    }
    func_ov006_020d4968(m, 1, 1);
    if (m->busy == 0) {
        for (i = 1; i < 9; i++) {
            if (m->items[i] != 0) {
                if (m->kind == 0x2b && i == 1 && m->sel > 0) {
                    continue;
                }
                if (m->kind == 0x18 && i == 1 && m->sel > 0) {
                    continue;
                }
                func_ov006_020d4944(m->items[i], m->sel, 1, 1);
            }
        }
        if (m->sel == 0) {
            func_0205964c(m);
        }
    }
}
