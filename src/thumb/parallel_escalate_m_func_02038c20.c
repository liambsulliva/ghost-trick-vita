struct Self02038c20 {
    char pad[0xa];
    unsigned short index;
};

struct Rect02038c20 {
    unsigned short x;
    unsigned short y;
};

extern struct Rect02038c20 data_0209a758[];
extern void *func_02010b7c(void);
extern unsigned int func_02010b60(void *object);
extern unsigned int func_02010b64(void *object);

int func_02038c20(struct Self02038c20 *self)
{
    unsigned int x;
    unsigned int y;
    unsigned int i;

    x = func_02010b60(func_02010b7c());
    y = func_02010b64(func_02010b7c());
    for (i = 0; i < 5; i++) {
        if (data_0209a758[i].x < x && (int)x < (int)(data_0209a758[i].x + 0x7a) &&
            data_0209a758[i].y < y && (int)y < (int)(data_0209a758[i].y + 0x1a)) {
            self->index = i;
            return 1;
        }
    }
    return 0;
}
