class File47f4 {
public:
    virtual void s0();
    virtual void s1();
    virtual int Seek(int pos);
    virtual int Read(void *buf, int size);
    virtual void s4();
    virtual void Reset();
};

typedef struct Track47f4 {
    int start;
    int offset;
} Track47f4;

typedef struct Stream47f4 {
    File47f4 *file;
    char pad4[0x2c];
    unsigned int count;
    char pad34[0x3c];
    void *buffer;
    int pad74;
    unsigned int blocks;
    int pad7c;
    int pos;
    int pad84;
    unsigned int rest;
    int pad8c;
    int ready;
    Track47f4 *tracks;
    int start;
    char pad9c[0x38];
    int base;
} Stream47f4;

extern "C" int func_ov004_020d47f4(Stream47f4 *s, int track);

int func_ov004_020d47f4(Stream47f4 *s, int track)
{
    unsigned int header;

    if (track < 0 || track >= s->count) {
        return 0;
    }
    if (s->file->Seek(s->base + s->tracks[track].offset) == 0) {
        return 0;
    }
    s->start = s->tracks[track].start;
    s->file->Reset();
    s->ready = 1;
    s->file->Read(&header, 4);
    s->blocks = header >> 14;
    s->rest = header & 0x3fff;
    s->pos = 0;
    s->file->Read(s->buffer, s->blocks + 4);
    return 1;
}
