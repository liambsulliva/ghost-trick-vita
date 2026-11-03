class Reader {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual int s4(char *buf, int size);
    virtual void s5();
};

struct Frame {
    char *data;
    char pad4[8];
    int a[6];
    int b[6];
};

struct Stream {
    Reader *reader;
    char pad4[8];
    unsigned int total;
    char pad10[0xe];
    unsigned short scale;
    char pad20[0x14];
    struct Frame *frame;
    char pad38[0x24];
    int *table_a;
    int *table_b;
    char pad64[0xc];
    char *buf[2];
    int size[2];
    int base[2];
    int count[2];
    int index;
    char pad94[4];
    unsigned int done;
    char pad9c[0x10];
    int ids[6];
    char padc4[4];
    int length;
};

extern "C" int func_ov004_020d46bc(struct Stream *s);

int func_ov004_020d46bc(struct Stream *s)
{
    unsigned int word;
    int last;
    int i;

    if (s->done == s->total) {
        return 0;
    }
    s->done++;
    last = s->index;
    s->index = (last + 1) % 2;
    s->reader->s5();
    word = *(unsigned int *)(s->buf[s->index] + (s->base[s->index] + s->size[s->index]));
    s->size[last] = word >> 14;
    s->count[last] = word & 0x3fff;
    s->frame->data = s->buf[s->index] + s->base[s->index];
    if (s->done < s->total) {
        s->base[last] = s->reader->s4(s->buf[last], s->size[last] + 4);
    }
    for (i = 0; i < 6; i++) {
        s->frame->a[i] = s->table_a[s->ids[i]];
        s->frame->b[i] = s->table_b[s->ids[i]];
    }
    s->length = s->count[s->index] * s->scale;
    return 1;
}
