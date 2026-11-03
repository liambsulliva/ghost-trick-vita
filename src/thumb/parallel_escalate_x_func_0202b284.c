struct AreaPair {
    int x;
    int y;
};

struct AreaSource {
    int a;
    int b;
    int c;
    int d;
    struct AreaPair pos;
};

struct AreaDest {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    struct AreaPair pos;
    int one;
};

void func_0202b284(struct AreaDest *dst, const struct AreaSource *src)
{
    dst->a = src->a;
    dst->b = src->c;
    dst->c = 0;
    dst->d = src->d;
    dst->f = 0;
    dst->e = src->b;
    dst->pos.x = src->pos.x;
    dst->pos.y = src->pos.y;
    dst->one = 0x1000;
}
