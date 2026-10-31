struct Rect {
    char pad[0xc];
    int x;
    int y;
    int w;
    int h;
    void *node;
};

extern void *func_02010b7c(void);
extern int func_0202cb64(void *object);
extern int func_02010b60(void *object);
extern int func_02010b64(void *object);
extern int func_0200fb4c(void *object);
extern int func_020283d0(void *object);
extern int func_02067e94(int value);

int func_02067e2c(struct Rect *p)
{
    int tx;
    int ty;
    int left;
    int top;
    int right;
    int bottom;

    if (func_0202cb64(func_02010b7c()) != 0) {
        return 0;
    }
    tx = func_02010b60(func_02010b7c());
    ty = func_02010b64(func_02010b7c());
    left = func_02067e94(func_0200fb4c(p->node)) - p->x;
    top = func_02067e94(func_020283d0(p->node)) - p->y;
    right = left + p->w;
    bottom = top + p->h;
    if ((((left - tx) ^ (right - tx)) & ((top - ty) ^ (bottom - ty))) >> 31) {
        return 1;
    }
    return 0;
}
