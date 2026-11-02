struct WaveFlags {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
};

struct WaveEntry {
    int dist;
    int obj;
    int pos[3];
    int extra;
};

struct Wave {
    unsigned char pad0[0x7c];
    struct WaveFlags flags;
    struct WaveFlags flags2;
    unsigned char state;
    unsigned char count;
    unsigned char age;
    unsigned char hold;
    unsigned char tries;
    unsigned char pad83[0x15];
    int head[3];
    unsigned char pada4[4];
    int reach;
    unsigned char padac[0x24];
    unsigned char *lines;
    unsigned char padd4[0x18];
    unsigned short busy;
    unsigned char padee[2];
    struct WaveEntry *entries;
    unsigned char padf4[4];
    int extra;
    unsigned char padfc[4];
    int target;
};

extern void *func_02008444(void);
extern int func_02010b38(void *game);
extern void func_020618cc(struct Wave *p);
extern void func_0205fe2c(int *out, struct Wave *p, int value, int held);
extern void func_020083c0(int *dst, void *a, void *b);
extern int func_020452e8(int *v);
extern void func_020083b0(int *dst, void *src);
extern void func_0206167c(struct Wave *p);
extern int func_0201f0d4(int *a, int *b);
extern int func_02036f3c(int object);
extern int func_02010194(void *game);
extern int func_0206ef28(int a, int b);
extern void func_020349b0(int *dst, int *src, int scale);
extern void func_02008378(int *dst, void *a, int *b);
extern int func_0201f0dc(int *v);
extern void func_02060f34(struct Wave *p);
extern void func_02061718(struct Wave *p, int object);

void func_0205fa2c(struct Wave *p, int value, int held)
{
    int pos[3];
    int diff[3];
    int dir[3];
    int rel[3];
    int pt[3];
    int off[3];
    int proj[3];
    int found;
    int len;
    int best_dist;
    struct WaveEntry *e;
    unsigned int i;
    int best;
    int dist;
    int t;

    if (!p->flags.b5 && !p->flags2.b3) {
        return;
    }
    if (func_02010b38(func_02008444()) != 0 && !p->flags2.b3) {
        return;
    }
    if (p->tries < 3) {
        p->tries++;
        if (!p->flags2.b3) {
            return;
        }
    }
    if (p->hold != 0) {
        p->hold--;
        return;
    }
    if (value == 0 && held == 0 && !p->flags2.b3) {
        p->flags.b4 = 0;
        p->flags.b6 = 0;
        return;
    }
    if (p->state == 5 || p->state == 6 || p->state == 7 || p->state == 8 || p->state == 3) {
        return;
    }
    if (p->state == 4) {
        if (!p->flags.b4 && p->flags2.b0 == 1) {
            func_020618cc(p);
        }
        return;
    }
    if (p->state == 2) {
        p->flags.b4 = 0;
        p->flags2.b3 = 0;
        return;
    }
    func_0205fe2c(pos, p, value, held);
    func_020083c0(diff, pos, p->lines + (p->count - 1) * 12);
    dist = func_020452e8(diff);
    if (!p->flags.b4 && !p->flags2.b3 && dist > 0x10000 && held == 0 && !p->flags.b6) {
        return;
    }
    if (p->state == 1 && p->target != 0 && !p->flags2.b3) {
        return;
    }
    p->flags.b4 = 1;
    p->flags.b0 = 1;
    p->flags.b1 = 0;
    if (!p->flags2.b3) {
        func_020083b0(p->head, pos);
    }
    func_0206167c(p);
    found = 0;
    func_020083c0(dir, p->lines + (p->count - 1) * 12, p->lines);
    len = func_0201f0d4(dir, dir);
    best_dist = 0x1000000;
    best = -1;
    if (len > 0x29) {
        for (i = 0; i < p->busy; i++) {
            if (func_02036f3c(p->entries[i].obj) != 0) {
                continue;
            }
            e = &p->entries[i];
            if (e->obj == func_02010194(func_02008444())) {
                continue;
            }
            if (e->dist > 0x10000) {
                if (e->dist > 0x23000) {
                    continue;
                }
                func_020083c0(rel, e->pos, p->lines);
                t = func_0206ef28(func_0201f0d4(dir, rel), len);
                if (t > 0x1000 || t < 0) {
                    continue;
                }
                func_020349b0(proj, dir, t);
                func_02008378(pt, p->lines, proj);
                func_020083c0(off, pt, p->entries[i].pos);
                if (func_0201f0dc(off) > 0x3000) {
                    continue;
                }
            }
            if (p->reach < 0x20000 && p->entries[i].dist > 0x8000) {
                continue;
            }
            if (p->reach < 0x10000 && p->entries[i].dist > 0x4000) {
                continue;
            }
            if (p->reach < 0x8000 && p->entries[i].dist > 0x2000) {
                continue;
            }
            if (p->reach < 0x4000 && p->entries[i].dist > 0x1000) {
                continue;
            }
            if (best == -1 || p->entries[i].dist < best_dist) {
                best = i;
                best_dist = p->entries[i].dist;
            }
        }
    }
    if (best != -1) {
        found = p->entries[best].obj;
        p->extra = p->entries[best].extra;
        func_020083b0(p->head, p->entries[best].pos);
    }
    p->target = found;
    if (found == 0) {
        p->flags.b0 = 0;
    }
    func_02060f34(p);
    func_02061718(p, found);
}
