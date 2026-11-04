struct Adpcm3a54 {
    int pred;
    int index;
};

extern const signed char data_020d7acc[];
extern const short data_020d7adc[];

void func_ov004_020d3a54(struct Adpcm3a54 *s, unsigned char *in, int count, unsigned int *out)
{
    const short *steps;
    int i;
    int code;
    int step;
    int diff;
    int lo;
    int old;

    i = 0;
    if (count <= 0) {
        return;
    }
    do {
        steps = data_020d7adc;
        code = in[i];
        old = s->index;
        s->index += data_020d7acc[code & 7];
        if (s->index < 0) {
            s->index = 0;
        }
        if (s->index > 88) {
            s->index = 88;
        }
        step = steps[old];
        diff = step >> 3;
        if (code & 4) {
            diff += step;
        }
        if (code & 2) {
            diff += step >> 1;
        }
        if (code & 1) {
            diff += step >> 2;
        }
        if (code & 8) {
            s->pred -= diff;
        } else {
            s->pred += diff;
        }
        if (s->pred < -32768) {
            s->pred = -32768;
        }
        if (s->pred > 32767) {
            s->pred = 32767;
        }
        lo = s->pred;
        code >>= 4;
        step = steps[s->index];
        s->index += data_020d7acc[code & 7];
        if (s->index < 0) {
            s->index = 0;
        }
        if (s->index > 88) {
            s->index = 88;
        }
        diff = step >> 3;
        if (code & 4) {
            diff += step;
        }
        if (code & 2) {
            diff += step >> 1;
        }
        if (code & 1) {
            diff += step >> 2;
        }
        if (code & 8) {
            s->pred -= diff;
        } else {
            s->pred += diff;
        }
        if (s->pred < -32768) {
            s->pred = -32768;
        }
        if (s->pred > 32767) {
            s->pred = 32767;
        }
        *out++ = (lo & 0xffff) | (s->pred << 16);
        i++;
    } while (i < count);
}
