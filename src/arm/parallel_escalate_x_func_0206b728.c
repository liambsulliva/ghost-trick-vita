typedef struct FSFile0206b728 {
    struct FSFile0206b728 *next;
    struct FSFile0206b728 *prev;
    void *arc;
    unsigned int stat;
    char pad10[8];
    int queue[2];
    char pad20[0x28];
} FSFile0206b728;

typedef struct FSArchive0206b728 {
    char pad0[8];
    FSFile0206b728 *list;
    int queue[2];
    unsigned int flag;
} FSArchive0206b728;

extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);
extern void func_0206b2ac(FSFile0206b728 *file, int result);
extern int func_0206b3a8(FSFile0206b728 *file, int command);
extern void func_0206c440(FSFile0206b728 *file);
extern void func_02074120(int *queue);

static inline int file_stat_bits(const FSFile0206b728 *file, unsigned int mask)
{
    return file->stat & mask;
}

FSFile0206b728 *func_0206b728(FSArchive0206b728 *arc, int owner)
{
    FSFile0206b728 *next;
    unsigned int bak;
    FSFile0206b728 *p;
    FSFile0206b728 *q;
    int start;
    FSFile0206b728 tmp;

    next = 0;
    bak = func_02075c98();
    if (arc->flag & 0x20) {
        arc->flag &= ~0x20;
        for (p = arc->list; p != 0; p = q) {
            q = p->next;
            if (((p->stat & 2) ? 1 : 0) != 0) {
                if (!file_stat_bits(p, 0x40)) {
                    func_0206b2ac(p, 3);
                    if (q == 0) {
                        q = arc->list;
                    }
                }
            }
        }
    }
    func_02075cac(bak);
    bak = func_02075c98();
    if (!(arc->flag & 0x40) && !(arc->flag & 0x8) && arc->list != 0) {
        start = owner && !(arc->flag & 0x10);
        if (start) {
            arc->flag |= 0x10;
        }
        func_02075cac(bak);
        if (start) {
            func_0206b3a8(arc->list, 9);
        }
        bak = func_02075c98();
        if (owner || start) {
            next = arc->list;
            next->stat |= 0x40;
        }
        if (owner) {
            if (next->stat & 0x4) {
                func_02074120(next->queue);
                next = 0;
            }
        }
        func_02075cac(bak);
    } else {
        if (owner) {
            if (arc->flag & 0x10) {
                func_0206c440(&tmp);
                tmp.arc = arc;
                arc->flag &= ~0x10;
                func_0206b3a8(&tmp, 10);
            }
            if (arc->flag & 0x40) {
                arc->flag = (arc->flag & ~0x40) | 0x8;
                func_02074120(arc->queue);
            }
        }
        func_02075cac(bak);
    }
    return next;
}
