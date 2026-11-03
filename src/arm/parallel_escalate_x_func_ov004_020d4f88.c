struct FSFile {
    char pad[0x24];
    unsigned int top;
    unsigned int bottom;
    unsigned int pos;
};

struct BufFile {
    int f0;
    int size;
    int pos;
    struct FSFile *file;
    unsigned char dirty;
};

int func_ov004_020d4f88(struct BufFile *p, struct FSFile *file)
{
    p->file = file;
    p->size = file->bottom - file->top;
    p->pos = p->file->pos - p->file->top;
    p->dirty = 0;
    return 1;
}
