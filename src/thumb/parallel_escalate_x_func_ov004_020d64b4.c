typedef struct Stream64b4 {
    char pad0[0x30];
    unsigned long long pos;
    void *left;
    void *right;
    unsigned int offset;
    unsigned int blocks;
    char pad48[0xac];
    unsigned int unit;
    char padf8[0x8c4];
    int queue[0x2b];
    int busy;
    int stamp;
    char pada70[0x38];
    unsigned char lowaa8 : 5;
    unsigned char active : 1;
    unsigned char paused : 1;
    unsigned char highaa8 : 1;
    char padaa9;
    unsigned char lowaaa : 3;
    unsigned char channels : 2;
    unsigned char highaaa : 3;
} Stream64b4;

extern int data_020bbb54;
extern volatile unsigned long long data_020bbb58;
extern void func_ov004_020d5b38(Stream64b4 *s);
extern void func_020740d0(void *queue);
extern void func_ov004_020d57b0(void *channel, unsigned int size);
extern void func_020747e4(void);
extern void func_ov004_020d5a04(Stream64b4 *s);

void func_ov004_020d64b4(Stream64b4 *s)
{
    if (s->active == 1) {
        func_ov004_020d5b38(s);
    }
    func_020740d0(s->queue);
    s->busy = 0;
    s->stamp = data_020bbb54;
    if (s->active == 1 && !s->paused) {
        if (s->channels == 1) {
            func_ov004_020d57b0(s->left, s->blocks * s->unit * 2);
        } else {
            func_ov004_020d57b0(s->left, s->blocks * s->unit * 2);
            func_ov004_020d57b0(s->right, s->blocks * s->unit * 2);
        }
        func_020747e4();
        if (s->pos > data_020bbb58) {
            s->pos -= data_020bbb58;
        } else {
            s->pos = 0;
        }
        data_020bbb58 = 0;
        s->offset = (unsigned int)s->pos * s->unit;
        if (s->offset == s->blocks * s->unit) {
            s->offset = 0;
        }
        func_ov004_020d5a04(s);
    }
}
