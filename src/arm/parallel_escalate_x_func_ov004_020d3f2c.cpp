class Stream {
public:
    virtual void s0();
    virtual void s1();
    virtual int Seek(int pos);
    virtual int Read(void *buf, int size);
    int Tell() { return pos; }
    int w4;
    int pos;
};

struct Header {
    signed char magic[5];
    signed char ver;
    unsigned short size;
    int w08;
    unsigned int w0c;
    unsigned int w10;
    int w14;
    unsigned short type;
    unsigned short count;
    int w1c;
    int w20;
    int w24;
    int w28;
    int w2c;
};

struct Rec {
    unsigned char pad000[0x1cc];
    int state;
    unsigned char pad1d0[0x8c0 - 0x1d0];
    unsigned char data[0xc34];
    int flag;
};

struct Obj {
    int w00;
    unsigned int w04;
    unsigned int w08;
    unsigned char pad0c[0x3c - 0x0c];
    int w3c;
    int w40;
    int w44;
};

struct Mod {
    Stream *stream;
    struct Header hdr;
    struct Obj *obj;
    int w38;
    unsigned char pad3c[0x4c - 0x3c];
    unsigned int w4c;
    unsigned int w50;
    int w54;
    struct Rec *recs;
    void **w5c;
    void **w60;
    void *w64;
    unsigned char pad68[0x70 - 0x68];
    void *w70[2];
    unsigned int w78;
    int w7c;
    int w80;
    int w84;
    unsigned int w88;
    int w8c;
    int w90;
    void *w94;
    int w98;
    int w9c;
    int wa0;
    int wa4;
    unsigned int wa8;
    unsigned int wac[6];
    int wc4;
    unsigned char padc8[0xd4 - 0xc8];
    int base;
};

extern "C" {
extern void *func_ov004_020d6b58(unsigned int size);
extern void func_02072f08(int value, void *dst, unsigned int size);
extern void func_02072fa8(void *dst, int value, unsigned int size);
extern int func_ov004_020d4da4(void);
extern int func_ov004_020d4e20(void);
extern int func_ov004_020d4f00(void);
extern int func_ov004_020d4e90(void);
}

extern "C" int func_ov004_020d3f2c(struct Mod *p, Stream *stream, unsigned int count);

int func_ov004_020d3f2c(struct Mod *p, Stream *stream, unsigned int count)
{
    unsigned int word;
    signed char tag[2];
    unsigned short idx;
    unsigned int i;
    int j;
    int k;
    int pos;
    int m;

    p->stream = stream;
    p->base = stream->pos;
    if (p->stream->Read(&p->hdr, 0x30) == 0) {
        return 0;
    }
    if (p->hdr.magic[0] != 'M' || p->hdr.magic[1] != 'O' || p->hdr.magic[2] != 'D' || p->hdr.magic[3] != 'S' || p->hdr.magic[4] != 'N' || (p->hdr.ver != '2' && p->hdr.ver != '3') || p->hdr.size != 10) {
        return 0;
    }
    if (p->hdr.magic[4] == 'N' && p->hdr.ver == '3') {
        do {
            if (p->stream->Read(tag, 2) == 0) {
                return 0;
            }
            if (p->stream->Read(&idx, 2) == 0) {
                return 0;
            }
            if (p->stream->Seek(p->stream->Tell() + idx * 4) == 0) {
                return 0;
            }
        } while (tag[0] != 'H' || tag[1] != 'E');
    }
    if (p->hdr.type != 0 && p->hdr.type != 1 && p->hdr.type != 2 && p->hdr.type != 3 && p->hdr.type != 4) {
        return 0;
    }
    p->obj = (struct Obj *)func_ov004_020d6b58(0x454);
    func_02072f08(0, p->obj, 0x454);
    p->w38 = func_ov004_020d4da4();
    p->obj->w3c = func_ov004_020d4e20();
    p->obj->w40 = 0x020d98fc;
    p->obj->w44 = func_ov004_020d4f00() + 0x40;
    p->w4c = p->hdr.w0c;
    p->w50 = p->hdr.w10;
    p->w54 = func_ov004_020d4e90();
    p->obj->w04 = p->hdr.w0c;
    p->obj->w08 = p->hdr.w10;
    if (p->hdr.w0c > 0x100) {
        return 0;
    }
    if (count < 6) {
        return 0;
    }
    p->wa8 = count;
    p->w5c = (void **)func_ov004_020d6b58(count << 2);
    if (p->w5c == 0) {
        return 0;
    }
    p->w60 = (void **)func_ov004_020d6b58(p->wa8 << 2);
    if (p->w60 == 0) {
        return 0;
    }
    for (i = 0; i < p->wa8; i++) {
        p->w5c[i] = func_ov004_020d6b58(p->hdr.w10 << 8);
        if (p->w5c[i] == 0) {
            return 0;
        }
        p->w60[i] = func_ov004_020d6b58((p->hdr.w10 >> 1) << 8);
        if (p->w60[i] == 0) {
            return 0;
        }
        func_02072f08(0, p->w5c[i], p->hdr.w10 << 8);
        func_02072fa8(p->w60[i], 0x80, (p->hdr.w10 >> 1) << 8);
    }
    p->w64 = func_ov004_020d6b58(p->wa8 << 2);
    if (p->w64 == 0) {
        return 0;
    }
    p->w98 = 0;
    p->wac[0] = 0;
    for (j = 1; j < 6; j++) {
        p->wac[j] = p->wa8 - j;
    }
    p->wc4 = 0;
    p->wa0 = 0;
    p->w9c = 0;
    for (j = 0; j < 2; j++) {
        p->w70[j] = func_ov004_020d6b58(p->hdr.w20 + 0x400);
        if (p->w70[j] == 0) {
            return 0;
        }
    }
    if (p->hdr.type != 0) {
        p->recs = (struct Rec *)func_ov004_020d6b58(p->hdr.count * 0x14f8);
        if (p->recs == 0) {
            return 0;
        }
        func_02072f08(0, p->recs, p->hdr.count * 0x14f8);
    }
    pos = p->stream->pos;
    p->stream->Seek(p->hdr.w28 + p->base);
    p->w94 = func_ov004_020d6b58(p->hdr.w2c << 3);
    if (p->stream->Read(p->w94, p->hdr.w2c << 3) == 0) {
        return 0;
    }
    p->stream->Seek(pos);
    if (p->hdr.type == 1) {
        int pos2;
        pos2 = p->stream->pos;
        p->stream->Seek(p->hdr.w24 + p->base);
        for (m = 0; m < p->hdr.count; m++) {
            if (p->stream->Read(p->recs[m].data, 0xc34) == 0) {
                return 0;
            }
            p->recs[m].flag = 2;
        }
        p->stream->Seek(pos2);
    } else if (p->hdr.type == 2) {
        for (k = 0; k < p->hdr.count; k++) {
            p->recs[k].state = 2;
        }
    }
    p->stream->Read(&word, 4);
    p->w78 = word >> 14;
    p->w88 = word & 0x3fff;
    p->w80 = 0;
    p->stream->Read(p->w70[0], p->w78 + 4);
    p->w90 = 1;
    return 1;
}
