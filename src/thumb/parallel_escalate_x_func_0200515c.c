typedef struct Hist0515c {
    short sample;
    short index;
} Hist0515c;

typedef struct Stream0515c {
    char pad0[0x50];
    int skip;
    int mute;
    int pad58;
    unsigned int data;
    int pad60;
    unsigned int pos;
    Hist0515c hist[2];
    char pad70[0x30];
    unsigned char format;
    unsigned char loop;
    unsigned char tracks;
    char pada3[5];
    unsigned int loop_start;
    int padac;
    unsigned int base;
    unsigned int blocks;
    unsigned int block_size;
    unsigned int block_samples;
    unsigned int last_size;
    unsigned int last_samples;
    char padc8[0x20];
    unsigned int flags;
    char padec[9];
    unsigned char channels;
    char padf6[0x2aa];
    unsigned char head;
    unsigned char tail;
} Stream0515c;

typedef struct Bank0515c {
    char pad0[0x4c48];
    unsigned char *buf;
} Bank0515c;

typedef struct Game0515c {
    char pad0[0x10];
    Bank0515c *bank;
} Game0515c;

extern Game0515c data_020b18a0;
extern void func_02006084(Stream0515c *stream);
extern void func_020010a0(void *dest, unsigned int size);
extern int func_0206c6d8(Stream0515c *file, int pos, int mode);
extern unsigned int func_0206c704(Stream0515c *file, void *dst, unsigned int len);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern void func_0207487c(void *dst, unsigned int size);
extern short func_020054a8(int nibble, Hist0515c *hist);

void func_0200515c(Stream0515c *s, unsigned int len, unsigned char **bufs)
{
    int end;
    unsigned int bsz;
    unsigned int fpos;
    long offset;
    unsigned int bytes;
    unsigned int remain;
    Hist0515c *hist;
    unsigned int j;
    short *out;
    unsigned int rd;
    unsigned int last;
    unsigned int ch;
    unsigned char *buf;
    unsigned char *dst;
    unsigned int bs;
    unsigned int pos;
    unsigned int samples;
    unsigned int skip;
    unsigned int i;
    unsigned int off;
    unsigned int n;
    unsigned int block;
    unsigned int got;

    rd = len;
    if (s->flags & 0x10) {
        return;
    }
    offset = 0;
    remain = len;
    while (remain != 0) {
        if ((s->flags & 0x20) && remain != 0) {
            func_02006084(s);
        }
        if (s->flags & 0x10) {
            for (ch = 0; ch < s->channels; ch++) {
                func_020010a0(bufs[ch] + offset, remain);
            }
            break;
        }
        bs = s->block_samples;
        pos = s->pos;
        block = pos / bs;
        if (block < s->blocks - 1) {
            bsz = s->block_size;
            samples = bs;
        } else {
            bsz = s->last_size;
            samples = s->last_samples;
        }
        off = pos - block * bs;
        n = remain;
        if (s->format != 0) {
            n = remain / 2;
        }
        if (s->skip != 0) {
            if (off == 0) {
                s->skip = 0;
            } else {
                n = off;
                off = 0;
            }
        }
        end = 0;
        if (off + n >= samples) {
            n = samples - off;
            if (block >= s->blocks - 1) {
                if (s->loop) {
                    end = 1;
                } else {
                    s->flags |= 0x10;
                }
                if (s->head != s->tail) {
                    s->flags |= 0x30;
                    end = 0;
                }
            }
        }
        skip = off;
        bytes = n;
        switch (s->format) {
        case 0:
            rd = n;
            break;
        case 1:
            skip = off * 2;
            bytes = n * 2;
            rd = n * 2;
            break;
        case 2:
            skip = off / 2;
            rd = (off + n + 1) / 2 - skip;
            if (off == 0) {
                rd += 4;
            } else {
                skip += 4;
            }
            bytes *= 2;
            break;
        }
        fpos = skip + block * s->block_size * s->tracks;
        fpos += s->data;
        fpos += s->base;
        for (ch = 0; ch < s->channels; ch++) {
            dst = bufs[ch] + offset;
            out = (short *)dst;
            if (ch < s->tracks) {
                if (s->format == 2) {
                    dst = data_020b18a0.bank->buf;
                }
                func_0206c6d8(s, fpos + ch * bsz, 0);
                got = func_0206c704(s, dst, rd);
                if (got != rd) {
                    bytes = 0;
                    end = 0;
                    n = 0;
                    s->flags |= 0x10;
                    break;
                }
                if (s->format == 2) {
                    last = off + n;
                    hist = &s->hist[ch];
                    buf = data_020b18a0.bank->buf;
                    dst = buf;
                    if (off == 0) {
                        hist->sample = ((unsigned short *)buf)[0];
                        dst = buf + 4;
                        hist->index = ((unsigned short *)buf)[1];
                    }
                    j = off;
                    if (off & 1) {
                        *out = func_020054a8((*dst >> 4) & 0xf, hist);
                        out++;
                        j++;
                        dst++;
                    }
                    while (j < (last & ~1)) {
                        *out++ = func_020054a8(*dst & 0xf, hist);
                        *out++ = func_020054a8((*dst >> 4) & 0xf, hist);
                        j += 2;
                        dst++;
                    }
                    if (j < last) {
                        *out = func_020054a8(*dst & 0xf, hist);
                    }
                }
            } else if (s->mute) {
                func_020010a0(dst, bytes);
            } else {
                func_0207303c(bufs[0] + offset, dst, bytes);
            }
        }
        if (s->skip != 0) {
            s->skip = 0;
        } else {
            if (end) {
                s->pos = s->loop_start;
            } else {
                s->pos += n;
            }
            offset += bytes;
            remain -= bytes;
        }
    }
    for (ch = 0; ch < s->channels; ch++) {
        func_0207487c(bufs[ch], len);
    }
}
