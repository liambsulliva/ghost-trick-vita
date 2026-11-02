typedef struct Regs0207d3f4 {
    unsigned int r[21];
} Regs0207d3f4;

typedef struct Ctx0207d3f4 {
    unsigned int f0;
    unsigned int f4;
    unsigned int f8;
    unsigned int fc;
    unsigned int f10;
    unsigned int f14;
    unsigned int base;
    Regs0207d3f4 regs;
} Ctx0207d3f4;

typedef struct Info0207d3f4 {
    unsigned int start;
    const unsigned char *table;
    const unsigned char *action;
    unsigned int first;
    unsigned int last;
    unsigned int f14;
} Info0207d3f4;

typedef struct Iter0207d3f4 {
    Info0207d3f4 info;
    Ctx0207d3f4 ctx;
} Iter0207d3f4;

typedef struct Catch0207d3f4 {
    unsigned int f0;
    unsigned int f4;
} Catch0207d3f4;

extern "C" int func_0207c8e8(Iter0207d3f4 *iter);
extern "C" int func_0207c904(Iter0207d3f4 *iter);
extern "C" const unsigned char *func_0207c674(const unsigned char *p, int *out);

extern "C" Catch0207d3f4 *func_0207d3f4(Ctx0207d3f4 *ctx, Info0207d3f4 *info)
{
    Iter0207d3f4 iter;
    int offset;
    Catch0207d3f4 *cb;
    int type;

    iter.info = *info;
    iter.ctx = *ctx;
    type = func_0207c8e8(&iter);
    for (;;) {
        switch (type) {
        case 1:
        case 14:
        default:
            return 0;
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
        case 12:
        case 15:
        case 16:
        case 17:
        case 18:
            type = func_0207c904(&iter);
            break;
        case 13:
            func_0207c674(iter.info.action + 1, &offset);
            cb = (Catch0207d3f4 *)(iter.ctx.base + offset);
            ctx->f0 = cb->f4;
            ctx->f4 = cb->f0;
            ctx->f8 = 0;
            ctx->fc = (unsigned int)cb;
            return cb;
        }
    }
}
