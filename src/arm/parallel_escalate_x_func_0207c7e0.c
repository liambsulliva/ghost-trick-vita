typedef struct Index0207c7e0 {
    unsigned int start;
    unsigned int flags;
    unsigned int table;
} Index0207c7e0;

typedef struct Info0207c7e0 {
    unsigned int start;
    const unsigned char *table;
    const unsigned char *action;
    Index0207c7e0 *first;
    Index0207c7e0 *last;
} Info0207c7e0;

extern int __FindExceptionTable(Info0207c7e0 *info, unsigned int pc);
extern Index0207c7e0 *func_0207c77c(Index0207c7e0 *first, int count, unsigned int pc);
extern const unsigned char *func_0207dff4(const unsigned char *p);
extern const unsigned char *func_0207c6f8(const unsigned char *p, unsigned int *out);

void func_0207c7e0(unsigned int pc, Info0207c7e0 *info)
{
    Index0207c7e0 *index;
    const unsigned char *p;
    unsigned int offset;
    unsigned int pos;
    unsigned int start;
    unsigned int length;
    unsigned int action;

    info->table = 0;
    info->action = 0;
    if (__FindExceptionTable(info, pc) == 0) {
        return;
    }
    index = func_0207c77c(info->first, info->last - info->first, pc);
    if (index == 0) {
        return;
    }
    if (index->flags & 1) {
        info->table = (const unsigned char *)&index->table;
    } else {
        info->table = (const unsigned char *)index->table;
    }
    info->start = index->start;
    offset = pc - index->start;
    p = func_0207dff4(info->table);
    pos = 0;
    do {
        p = func_0207c6f8(p, &start);
        if (start == 0) {
            return;
        }
        p = func_0207c6f8(p, &length);
        p = func_0207c6f8(p, &action);
        pos += start;
        if (offset < pos) {
            return;
        }
        pos += length;
    } while (offset > pos);
    info->action = info->table + action;
}
