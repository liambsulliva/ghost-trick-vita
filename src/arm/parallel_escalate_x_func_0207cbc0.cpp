typedef struct UnwindInfo {
    unsigned int start;
    const unsigned char *table;
    const unsigned char *action;
} UnwindInfo;

typedef struct UnwindContext {
    int f0;
    char *thrown;
    void *dtor;
    unsigned char pad0c[0xc];
    char *fp;
    int regs[16];
} UnwindContext;

typedef struct CatchInfo {
    char *location;
    int f4;
    void (*dtor)(void *obj, int flag);
} CatchInfo;

struct Tail16 {
    int offset2;
    int unused;
};

typedef void (*DtorFunc)(void *obj, int flag);
typedef void (*DeleteFunc)(void *obj);
typedef void (*PairFunc)(void *a, void *b);

extern "C" {
unsigned int func_0207ded8(UnwindContext *ctx, UnwindInfo *info);
void func_0207c7e0(unsigned int pc, UnwindInfo *info);
void func_0207da08(void);
void func_0207df34(UnwindContext *ctx, UnwindInfo *info);
const unsigned char *func_0207c674(const unsigned char *p, int *out);
const unsigned char *func_0207c6f8(const unsigned char *p, unsigned int *out);

}
extern "C" void func_0207cbc0(UnwindContext *ctx, UnwindInfo *info, const unsigned char *catcher)
{
    #pragma exception_terminate

    int s1_offset;
    int s2_local;
    int s3_local;
    int s3_cond;
    int s4_local;
    unsigned int s5_size;
    unsigned int s5_count;
    int s5_local;
    int s6_offset;
    int s6_obj;
    int s7_offset;
    int s7_obj;
    int s8_offset;
    int s8_obj;
    int s8_cond;
    unsigned int s9_size;
    unsigned int s9_count;
    int s9_offset;
    int s9_obj;
    int s10_local;
    int s11_local;
    int s11_cond;
    int s12_offset;
    unsigned int s12_count;
    int s13_local;
    int s15_offset;
    unsigned int s15_size;
    unsigned int s15_count;
    struct Tail16 s16_tail;
    int s16_offset;
    int s16_obj;
    int s17_offset2;
    int s17_offset;
    int s17_obj2;
    int s17_obj;
    unsigned int s18_size;
    int s18_count;
    int s18_obj;
    int s19_offset;
    const unsigned char *p;
    const unsigned char *q;
    const unsigned char *next;
    unsigned char action;

    for (;;) {
        if (info->action == 0) {
            func_0207c7e0(func_0207ded8(ctx, info), info);
            if (info->table == 0) {
                func_0207da08();
            }
            func_0207df34(ctx, info);
            if (info->action == 0) {
                continue;
            }
        }
        p = info->action;
        action = *p;
        switch (action & 0x1f) {
        case 1:
            {
                func_0207c674(p + 1, &s1_offset);
                info->action += s1_offset;
            }
            break;
        case 2:
            {
                q = func_0207c674(p + 1, &s2_local);
                ((DtorFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24)))(ctx->fp + s2_local, -1);
                info->action = q + 4;
            }
            break;
        case 3:
            {
                int inreg;
                DtorFunc fn;
                unsigned char c;
                inreg = *info->action & 0x40;
                q = func_0207c674(p + 1, &s3_cond);
                q = func_0207c674(q, &s3_local);
                fn = (DtorFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (inreg) {
                    c = ctx->regs[s3_cond];
                } else {
                    c = *(unsigned char *)(ctx->fp + s3_cond);
                }
                if (c != 0) {
                    fn(ctx->fp + s3_local, -1);
                }
                info->action = next;
            }
            break;
        case 4:
            {
                int inreg;
                DtorFunc fn;
                char *obj;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s4_local);
                fn = (DtorFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (inreg) {
                    obj = (char *)ctx->regs[s4_local];
                } else {
                    obj = *(char **)(ctx->fp + s4_local);
                }
                fn(obj, -1);
                info->action = next;
            }
            break;
        case 5:
            {
                char *ptr;
                DtorFunc fn;
                unsigned int n;
                q = func_0207c674(p + 1, &s5_local);
                q = func_0207c6f8(q, &s5_count);
                q = func_0207c6f8(q, &s5_size);
                fn = (DtorFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                n = s5_count;
                ptr = ctx->fp + s5_local + n * s5_size;
                while (n != 0) {
                    ptr -= s5_size;
                    fn(ptr, -1);
                    n--;
                }
                info->action = next;
            }
            break;
        case 6:
            {
                int inreg;
                DtorFunc fn;
                char *obj;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s6_obj);
                q = func_0207c674(q, &s6_offset);
                fn = (DtorFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (inreg) {
                    obj = (char *)ctx->regs[s6_obj];
                } else {
                    obj = *(char **)(ctx->fp + s6_obj);
                }
                fn(obj + s6_offset, 0);
                info->action = next;
            }
            break;
        case 7:
            {
                int inreg;
                DtorFunc fn;
                char *obj;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s7_obj);
                q = func_0207c674(q, &s7_offset);
                fn = (DtorFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (inreg) {
                    obj = (char *)ctx->regs[s7_obj];
                } else {
                    obj = *(char **)(ctx->fp + s7_obj);
                }
                fn(obj + s7_offset, -1);
                info->action = next;
            }
            break;
        case 8:
            {
                int inreg;
                DtorFunc fn;
                char *obj;
                short c;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s8_cond);
                q = func_0207c674(q, &s8_obj);
                q = func_0207c674(q, &s8_offset);
                fn = (DtorFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (action & 0x40) {
                    c = ctx->regs[s8_cond];
                } else {
                    c = *(short *)(ctx->fp + s8_cond);
                }
                if (c != 0) {
                    if (inreg) {
                        obj = (char *)ctx->regs[s8_obj];
                    } else {
                        obj = *(char **)(ctx->fp + s8_obj);
                    }
                    fn(obj + s8_offset, -1);
                }
                info->action = next;
            }
            break;
        case 9:
            {
                int inreg;
                char *ptr;
                DtorFunc fn;
                unsigned int n;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s9_obj);
                q = func_0207c674(q, &s9_offset);
                q = func_0207c6f8(q, &s9_count);
                q = func_0207c6f8(q, &s9_size);
                fn = (DtorFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (inreg) {
                    ptr = (char *)ctx->regs[s9_obj] + s9_offset;
                } else {
                    ptr = *(char **)(ctx->fp + s9_obj) + s9_offset;
                }
                n = s9_count;
                ptr += n * s9_size;
                while (n != 0) {
                    ptr -= s9_size;
                    fn(ptr, -1);
                    n--;
                }
                info->action = next;
            }
            break;
        case 10:
            {
                int inreg;
                DeleteFunc fn;
                char *obj;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s10_local);
                fn = (DeleteFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (inreg) {
                    obj = (char *)ctx->regs[s10_local];
                } else {
                    obj = *(char **)(ctx->fp + s10_local);
                }
                fn(obj);
                info->action = next;
            }
            break;
        case 11:
            {
                int inreg;
                DeleteFunc fn;
                char *obj;
                unsigned char c;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s11_cond);
                q = func_0207c674(q, &s11_local);
                fn = (DeleteFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (action & 0x40) {
                    c = ctx->regs[s11_cond];
                } else {
                    c = *(unsigned char *)(ctx->fp + s11_cond);
                }
                if (c != 0) {
                    if (inreg) {
                        obj = (char *)ctx->regs[s11_local];
                    } else {
                        obj = *(char **)(ctx->fp + s11_local);
                    }
                    fn(obj);
                }
                info->action = next;
            }
            break;
        case 12:
            if (catcher == p) {
                return;
            }
            {
                info->action = func_0207c674(func_0207c6f8(p + 5, &s12_count), &s12_offset);
            }
            break;
        case 13:
            {
                CatchInfo *ci;
                q = func_0207c674(p + 1, &s13_local);
                ci = (CatchInfo *)(ctx->fp + s13_local);
                if (ci->dtor != 0) {
                    if (ctx->thrown == ci->location) {
                        ctx->dtor = ci->dtor;
                    } else {
                        ci->dtor(ci->location, -1);
                    }
                }
                info->action = q;
            }
            break;
        case 15:
            if (catcher == p) {
                return;
            }
            {
                info->action = func_0207c674(func_0207c6f8(func_0207c6f8(p + 1, &s15_count), &s15_size), &s15_offset) + s15_count * 4;
            }
            break;
        case 16:
            {
                char *base;
                int inreg;
                PairFunc fn;
                char *obj;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s16_obj);
                q = func_0207c674(q, &s16_offset);
                base = (char *)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                q = func_0207c674(q + 4, &s16_tail.offset2);
                fn = (PairFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (inreg) {
                    obj = (char *)ctx->regs[s16_obj];
                } else {
                    obj = *(char **)(ctx->fp + s16_obj);
                }
                fn(obj + s16_offset, base + s16_tail.offset2);
                info->action = next;
            }
            break;
        case 17:
            {
                int inreg2;
                int inreg;
                PairFunc fn;
                char *obj;
                char *obj2;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s17_obj);
                q = func_0207c674(q, &s17_offset);
                inreg2 = *q++ & 0x20;
                q = func_0207c674(q, &s17_obj2);
                q = func_0207c674(q, &s17_offset2);
                fn = (PairFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (inreg) {
                    obj = (char *)ctx->regs[s17_obj];
                } else {
                    obj = *(char **)(ctx->fp + s17_obj);
                }
                if (inreg2) {
                    obj2 = (char *)ctx->regs[s17_obj2];
                } else {
                    obj2 = *(char **)(ctx->fp + s17_obj2);
                }
                fn(obj + s17_offset, obj2 + s17_offset2);
                info->action = next;
            }
            break;
        case 18:
            {
                int inreg;
                int inreg2;
                char *ptr;
                DtorFunc fn;
                unsigned int total;
                unsigned int n;
                inreg = *info->action & 0x20;
                q = func_0207c674(p + 1, &s18_obj);
                inreg2 = *q++ & 0x20;
                q = func_0207c674(q, &s18_count);
                q = func_0207c6f8(q, &s18_size);
                fn = (DtorFunc)(q[0] | (q[1] << 8) | (q[2] << 16) | (q[3] << 24));
                next = q + 4;
                if (inreg) {
                    ptr = (char *)ctx->regs[s18_obj];
                } else {
                    ptr = *(char **)(ctx->fp + s18_obj);
                }
                if (inreg2) {
                    total = ctx->regs[s18_count];
                } else {
                    total = *(unsigned int *)(ctx->fp + s18_count);
                }
                ptr += total;
                n = total / s18_size;
                while (n != 0) {
                    ptr -= s18_size;
                    fn(ptr, -1);
                    n--;
                }
                info->action = next;
            }
            break;
        case 19:
            {
                info->action = func_0207c674(p + 1, &s19_offset);
            }
            break;
        default:
            func_0207da08();
            break;
        }
        if (action & 0x80) {
            info->action = 0;
        }
    }
}
