extern int func_0206c290(void *file);
extern int func_0206c6d8(void *file, int offset, int mode);

struct BufFile {
    int f0;
    int f4;
    int pos;
    void *file;
    unsigned char dirty;
};

int func_ov004_020d4fc0(struct BufFile *p, int pos)
{
    if (p->dirty == 1) {
        func_0206c290(p->file);
        p->dirty = 0;
    }
    if (func_0206c6d8(p->file, pos, 0) == 0) {
        return 0;
    }
    p->pos = pos;
    return 1;
}
