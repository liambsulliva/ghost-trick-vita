typedef struct Stream02004fcc {
    char pad0[0x48];
    unsigned char index;
    char pad49;
    short sound;
    char pad4c[0xe8 - 0x4c];
    unsigned int flags;
    int state;
    int padf0;
    unsigned char result;
    char padf5[0xfc - 0xf5];
    unsigned short count;
    unsigned short chunks;
    int written;
    int consumed;
    char pad108[0x110 - 0x108];
    unsigned int chunks5;
    int bufs[2];
} Stream02004fcc;

typedef struct Game02004fcc {
    char pad[0x10];
    char *sys;
} Game02004fcc;

extern Game02004fcc data_020b18a0;
extern void func_020740d0(int value);
extern void func_02074588(void *mutex);
extern void func_020745d8(void *mutex);
extern void func_02006084(Stream02004fcc *stream);
extern void func_02005654(int sound);
extern void func_0200515c(Stream02004fcc *stream, int size, int *bufs);

void func_02004fcc(Stream02004fcc *param_1)
{
    int bufs[2];
    unsigned int n;
    unsigned int i;
    int j;

    while (1) {
        func_020740d0(0);
        func_02074588(data_020b18a0.sys + 0x1bcc);
        if (param_1->written - param_1->consumed < param_1->count && param_1->state == 6) {
            if ((param_1->flags & 0x20) != 0) {
                func_02006084(param_1);
            }
            if ((param_1->flags & 0x10) != 0) {
                func_02005654(param_1->sound);
                goto unlock;
            }
            if (param_1->state == 5) {
                n = param_1->chunks5;
            } else {
                n = param_1->chunks;
            }
            for (i = 0; i < n; i++) {
                j = 0;
                do {
                    bufs[j] = param_1->bufs[j] + param_1->index * 0x200;
                    j++;
                } while (j < 2);
                func_0200515c(param_1, 0x200, bufs);
                if (param_1->index == param_1->count - 1) {
                    param_1->index = 0;
                } else {
                    param_1->index = param_1->index + 1;
                }
                param_1->written = param_1->written + 1;
            }
            param_1->result = 3;
        } else {
            param_1->result = 2;
        }
    unlock:
        func_020745d8(data_020b18a0.sys + 0x1bcc);
    }
}
