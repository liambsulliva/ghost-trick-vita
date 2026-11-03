typedef struct Record0202ce88 {
    unsigned short key;
    unsigned short type;
    unsigned short id;
    short f6;
    unsigned char f8;
    char pad9[3];
    char posa[0x18];
    char posb[8];
} Record0202ce88;

class Actor0202ce88 {
public:
    virtual ~Actor0202ce88();
    virtual void v008();
    virtual void v00c();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01c();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02c();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03c();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04c();
    virtual void v050(int value);
    char f4;
    unsigned char f5;
    unsigned char f6;
    char f7;
    short f8;
    unsigned short fa;
    char padc[2];
    unsigned short fe;
    unsigned short f10;
    char pad12[6];
    char posb[0x10];
    char posa[0x50];
    Actor0202ce88 *next;
};

class Mgr0202ce88 {
public:
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00c();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01c();
    virtual void v020();
    virtual Actor0202ce88 *v024(int type);
    char pad04[0x70];
    Actor0202ce88 *actors;
    char pad78[0xe];
    unsigned char f86_0 : 1;
    unsigned char hidden : 1;
    unsigned char f86_rest : 6;
    char pad87;
    unsigned int index;
    char pad8c[0x2c];
    unsigned int count;
    int *offsets;
    char *base;
    char padc4[0x18];
    int fdc;
};

extern "C" {
extern Actor0202ce88 *data_020ca2d8[];
extern void func_0202cff0(void *object, int value);
extern void func_0202b4fc(Actor0202ce88 *actor, Mgr0202ce88 *mgr);
extern void func_0202b284(void *dest, const void *src);
extern void func_0202bcf0(void *dest, const void *src);

void func_0202ce88(Mgr0202ce88 *self)
{
    unsigned int index;
    char *data;
    int half;
    int count;
    Record0202ce88 *recs;
    Actor0202ce88 *actor;
    Actor0202ce88 *cur;
    Record0202ce88 *rec;
    int found;
    int lo;
    int hi;
    int mid;
    int prev;
    unsigned short key;
    unsigned int i;

    index = self->index;
    if (index >= self->count) {
        index = self->count - 1;
    }
    data = self->base + self->offsets[index];
    count = *(int *)(data + 4);
    recs = (Record0202ce88 *)((unsigned int)data + 8);
    func_0202cff0(data_020ca2d8, count * 4);
    half = (count - 1) >> 1;
    actor = self->actors;
    while (actor != 0) {
        found = 0;
        lo = -1;
        cur = actor;
        actor = actor->next;
        hi = count;
        key = cur->fe;
        mid = half;
        prev = -1;
        if (key != 0xffff) {
            while (prev != mid && mid >= 0) {
                prev = mid;
                rec = &recs[mid];
                if (key == rec->key) {
                    found = 1;
                    data_020ca2d8[mid] = cur;
                    break;
                }
                if (key > rec->key) {
                    lo = mid;
                } else {
                    hi = mid;
                }
                mid = (lo + hi) >> 1;
            }
            if (!found && cur != 0) {
                delete cur;
                cur = 0;
            }
        }
    }
    for (i = 0; i < count; i++) {
        rec = &recs[i];
        cur = data_020ca2d8[i];
        if (cur != 0 && rec->id != cur->fa) {
            if (cur != 0) {
                delete cur;
                cur = 0;
            }
        }
        if (cur == 0) {
            cur = self->v024(rec->type);
            if (cur == 0) {
                continue;
            }
            cur->fa = rec->id;
            cur->fe = rec->key;
            cur->f10 = rec->key;
            cur->f8 = rec->f6;
            func_0202b4fc(cur, self);
            if (cur->f5 == 0x27) {
                cur->v050(self->fdc);
            }
        }
        if (rec->f8 != 0 || self->hidden != 0 || cur->f6 != 0) {
            func_0202b284(cur->posa, rec->posa);
            func_0202bcf0(cur->posb, rec->posb);
        }
    }
}
}
