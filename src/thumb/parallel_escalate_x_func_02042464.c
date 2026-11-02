typedef struct Desc42464 {
    unsigned int flag : 1;
    unsigned int a : 12;
    unsigned int b : 12;
    unsigned int c : 7;
    unsigned int d : 20;
    unsigned int e : 12;
} Desc42464;

typedef struct Item42464 {
    unsigned char active;
    unsigned char shown;
    char pad2[2];
    Desc42464 *desc;
    char pad8[8];
    int timer;
    char pad14[4];
    char sub[0x54];
} Item42464;

typedef struct Holder42464 {
    char pad0[0x14];
    unsigned int count;
    Item42464 *items;
    char pad1c[4];
    struct Holder42464 *next;
} Holder42464;

typedef struct Root42464 {
    char pad0[0x58];
    Holder42464 *list;
} Root42464;

extern Root42464 data_020ce5c4;
extern unsigned int func_0204264c(void *sub);
extern unsigned int func_02042650(void *sub);
extern unsigned int func_02019094(void *sub);
extern unsigned int func_02019098(void *sub);
extern unsigned int func_0202c374(void *sub);
extern int func_0204224c(Item42464 *item);
extern int func_02029020(void *sub);
extern int func_02029024(void *sub);
extern int func_02042654(void *sub);
extern int func_02042658(void *sub);
extern int func_02042660(void *sub);
extern void func_020427b0(void *part, int data, int a, int b, int c, int d, int e, int f, int g, int h, int i);
extern void func_02042664(void *sub);
extern void func_02042a44(void *sub);
extern void func_02042ac8(void *sub);
extern void func_0202db3c(Item42464 *item);
extern void func_020421e8(Item42464 *item);

void func_02042464(void)
{
    Holder42464 *holder;
    unsigned int i;
    unsigned int j;
    Desc42464 *desc;
    int data;
    int a;
    int b;
    int c;
    int d;
    int e;
    unsigned int k;
    Holder42464 *h;
    unsigned int n;
    Holder42464 *g;

    for (holder = data_020ce5c4.list; holder != 0; holder = holder->next) {
        for (i = 0; i < holder->count; i++) {
            if (holder->items[i].active == 0 || holder->items[i].shown != 0) {
                continue;
            }
            desc = holder->items[i].desc;
            for (j = 0; j < holder->count; j++) {
                if (i == j) {
                    continue;
                }
                if (holder->items[j].active != 0 || holder->items[j].shown == 0) {
                    continue;
                }
                if (desc->d != func_0204264c(holder->items[j].sub)) {
                    continue;
                }
                if (desc->e != func_02042650(holder->items[j].sub)) {
                    continue;
                }
                if (desc->a != func_02019094(holder->items[j].sub)) {
                    continue;
                }
                if (desc->b != func_02019098(holder->items[j].sub)) {
                    continue;
                }
                if (desc->c != func_0202c374(holder->items[j].sub)) {
                    continue;
                }
                data = func_0204224c(&holder->items[i]);
                a = func_02029020(holder->items[j].sub);
                b = func_02029024(holder->items[j].sub);
                c = func_02042654(holder->items[j].sub);
                d = func_02042658(holder->items[j].sub);
                e = func_02042660(holder->items[j].sub);
                func_020427b0(holder->items[i].sub, data, 1, 0, 1, c, 0xffff, d, a, b, e);
                func_02042664(holder->items[i].sub);
                func_02042a44(holder->items[j].sub);
                func_02042ac8(holder->items[j].sub);
                holder->items[j].shown = 0;
                holder->items[i].shown = 1;
                break;
            }
        }
    }
    for (h = data_020ce5c4.list; h != 0; h = h->next) {
        for (k = 0; k < h->count; k++) {
            if (h->items[k].active == 0 && h->items[k].shown != 0) {
                h->items[k].timer--;
                if (h->items[k].timer < 0) {
                    func_0202db3c(&h->items[k]);
                }
            }
        }
    }
    for (g = data_020ce5c4.list; g != 0; g = g->next) {
        for (n = 0; n < g->count; n++) {
            func_020421e8(&g->items[n]);
        }
    }
}
