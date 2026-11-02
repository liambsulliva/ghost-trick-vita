typedef struct Entry02006084 {
    unsigned char data[0x40];
    short f40;
    char pad42[2];
    int f44;
    int f48;
    unsigned char f4c;
    unsigned char f4d;
    unsigned char f4e;
    unsigned char f4f;
} Entry02006084;

typedef struct Queue02006084 {
    char pad00[0x4a];
    short f4a;
    char pad4c[0xc];
    int f58;
    int f5c;
    unsigned char f60;
    unsigned char f61;
    unsigned char f62;
    unsigned char f63;
    int f64;
    char pad68[0x20];
    unsigned char buf[0x40];
    char padc8[0x20];
    unsigned int flags;
    char padec[0x34];
    Entry02006084 entries[8];
    unsigned char head;
    unsigned char tail;
} Queue02006084;

extern void func_0207303c(const void *src, void *dst, unsigned int size);

void func_02006084(Queue02006084 *q)
{
    Entry02006084 *e;

    q->flags &= ~0x20;
    if (q->head != q->tail) {
        e = &q->entries[q->head];
        q->head = (q->head + 1) & 7;
        func_0207303c(e, q->buf, 0x40);
        q->f4a = e->f40;
        q->f58 = e->f44;
        q->f60 = e->f4c;
        q->f61 = e->f4d;
        q->f62 = e->f4e;
        q->f63 = e->f4f;
        q->f5c = e->f48;
        q->f64 = 0;
        q->flags &= ~0x10;
    }
}
