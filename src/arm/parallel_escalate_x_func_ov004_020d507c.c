extern int func_0206c6d8(void *file, int offset, int mode);
extern int func_0206c758(void *file, void *buffer, int length);

struct BufFile {
    int f0;
    int f4;
    int pos;
    void *file;
    unsigned char dirty;
};

int func_ov004_020d507c(struct BufFile *p, void *buffer, int length)
{
    unsigned int pos;
    int skip;
    unsigned int n;

    pos = p->pos;
    skip = pos - (pos & 0xfffffe00);
    n = length + skip;
    if ((n & 0x1ff) != 0) {
        n = (n & 0xfffffe00) + 0x200;
    }
    func_0206c6d8(p->file, pos & 0xfffffe00, 0);
    if (func_0206c758(p->file, buffer, n) == -1) {
        return 0;
    }
    p->pos += length;
    p->dirty = 1;
    return skip;
}
