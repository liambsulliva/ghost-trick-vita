typedef struct Ctx0207ded8 {
    unsigned int pad0[5];
    unsigned int sp;
    unsigned int base;
    unsigned int regs[16];
    unsigned int pad5c;
    unsigned int offset;
    unsigned int pad64;
    unsigned short mask;
    unsigned char flag;
} Ctx0207ded8;

unsigned int func_0207ded8(Ctx0207ded8 *ctx, void *info)
{
    unsigned int offset;
    int skip;
    unsigned int *p;
    int i;

    offset = ctx->offset;
    skip = 16;
    if (ctx->flag == 0) {
        skip = 0;
    }
    p = (unsigned int *)(ctx->base + offset - skip);
    for (i = 15; i >= 0; i--) {
        if (ctx->mask & (1 << i)) {
            p--;
            ctx->regs[i] = *p;
        }
    }
    ctx->sp = ctx->base + offset;
    return ctx->regs[14];
}
