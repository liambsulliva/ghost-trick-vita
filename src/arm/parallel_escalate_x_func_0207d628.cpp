typedef struct Regs0207d628 {
    unsigned int r[21];
} Regs0207d628;

typedef struct Ctx0207d628 {
    unsigned int f0;
    unsigned int f4;
    unsigned int f8;
    unsigned int fc;
    unsigned int f10;
    unsigned int f14;
    unsigned int base;
    Regs0207d628 regs;
} Ctx0207d628;

typedef struct Info0207d628 {
    unsigned int start;
    const unsigned char *table;
    const unsigned char *action;
    unsigned int first;
    unsigned int last;
    unsigned int f14;
} Info0207d628;

typedef struct Iter0207d628 {
    Info0207d628 info;
    Ctx0207d628 ctx;
} Iter0207d628;

typedef struct Catch0207d628 {
    unsigned int type;
    unsigned int pcoffset;
    int cinfo;
} Catch0207d628;

typedef struct Spec0207d628 {
    unsigned int count;
    unsigned int pcoffset;
    int cinfo;
    const unsigned char *list;
} Spec0207d628;

extern "C" int func_0207c8e8(Iter0207d628 *iter);
extern "C" int func_0207c904(Iter0207d628 *iter);
extern "C" const unsigned char *func_0207c674(const unsigned char *p, int *out);
extern "C" const unsigned char *func_0207c6f8(const unsigned char *p, unsigned int *out);
extern "C" int func_0207da40(unsigned int a, unsigned int type, void *thrown);
extern "C" int func_0207d548(unsigned int a, Spec0207d628 *spec);
extern "C" void func_0207d5c8(Ctx0207d628 *ctx, Info0207d628 *info, Spec0207d628 *spec, const unsigned char *action);
extern "C" void func_0207da08(void);

extern "C" const unsigned char *func_0207d628(Ctx0207d628 *ctx, Info0207d628 *info, void *thrown)
{
    Iter0207d628 iter;
    Catch0207d628 catchblock;
    Spec0207d628 spec;
    const unsigned char *p;
    int type;

    iter.info = *info;
    iter.ctx = *ctx;
    type = func_0207c8e8(&iter);
    for (;;) {
        switch (type) {
        case 12:
            p = iter.info.action;
            catchblock.type = p[1] | (p[2] << 8) | (p[3] << 16) | (p[4] << 24);
            p = func_0207c6f8(p + 5, &catchblock.pcoffset);
            func_0207c674(p, &catchblock.cinfo);
            if (func_0207da40(ctx->f0, catchblock.type, thrown) == 0) {
                break;
            }
            goto done;
        case 15:
            p = func_0207c6f8(iter.info.action + 1, &spec.count);
            p = func_0207c6f8(p, &spec.pcoffset);
            spec.list = func_0207c674(p, &spec.cinfo);
            if (func_0207d548(ctx->f0, &spec) == 0) {
                func_0207d5c8(ctx, info, &spec, iter.info.action);
            }
            break;
        case 1:
        case 14:
        default:
            func_0207da08();
            goto done;
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 13:
        case 16:
        case 17:
        case 18:
        case 19:
            break;
        }
        type = func_0207c904(&iter);
    }
done:
    return iter.info.action;
}
