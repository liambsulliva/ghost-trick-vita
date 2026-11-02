typedef struct Track6ab4 {
    int *list;
    int pos;
    unsigned int count;
    int padc;
    int cur;
} Track6ab4;

extern unsigned int func_ov004_020d3d8c(void *obj, int index);
extern void func_ov004_020d3dec(void *obj, int index);

int func_ov004_020d6ab4(int unused, void *obj, Track6ab4 *t, int forward, unsigned char loop, int limit, int end)
{
    int result;
    int advance;
    int index;

    result = -1;
    if (forward != 0) {
        advance = 0;
        if (t->list[t->cur] + 1 < t->count) {
            if (func_ov004_020d3d8c(obj, t->list[t->cur] + 1) <= limit) {
                advance = 1;
            }
        } else if (end - 1 <= limit) {
            advance = 1;
        }
        if (advance) {
            t->cur++;
            index = t->list[t->cur];
            if (index >= 0) {
                func_ov004_020d3dec(obj, index);
                result = func_ov004_020d3d8c(obj, index);
                t->pos = index;
            } else if (loop != 0) {
                t->cur = 0;
                index = t->list[t->cur];
                func_ov004_020d3dec(obj, index);
                result = func_ov004_020d3d8c(obj, index);
                t->pos = index;
            } else {
                return -2;
            }
        }
    } else if (limit > 0) {
        if (t->pos + 1 < t->count && func_ov004_020d3d8c(obj, t->pos + 1) <= limit) {
            t->pos++;
        }
    } else {
        t->pos = 0;
    }
    return result;
}
