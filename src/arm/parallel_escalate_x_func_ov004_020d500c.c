extern int func_0206c290(void *file);
extern int func_0206c6d8(void *file, int offset, int mode);
extern int func_0206c704(void *file, void *dst, int len);

struct BufFile {
    int f0;
    int f4;
    int pos;
    void *file;
    unsigned char dirty;
};

int func_ov004_020d500c(struct BufFile *p, void *buf, int len)
{
    if (p->dirty == 1) {
        func_0206c290(p->file);
        func_0206c6d8(p->file, p->pos, 0);
        p->dirty = 0;
    }
    if (func_0206c704(p->file, buf, len) == -1) {
        return 0;
    }
    p->pos += len;
    return 1;
}
