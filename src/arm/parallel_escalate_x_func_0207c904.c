typedef struct Info0207c904 {
    unsigned int start;
    const unsigned char *table;
    const unsigned char *action;
    unsigned int first;
    unsigned int last;
    unsigned int f14;
} Info0207c904;

typedef struct Ctx0207c904 {
    unsigned int words[28];
} Ctx0207c904;

typedef struct Iter0207c904 {
    Info0207c904 info;
    Ctx0207c904 ctx;
} Iter0207c904;

typedef struct Count0207c904 {
    int unused;
    unsigned int count;
} Count0207c904;

extern unsigned int func_0207ded8(Ctx0207c904 *ctx, Info0207c904 *info);
extern void func_0207c7e0(unsigned int pc, Info0207c904 *info);
extern int func_0207df34(Ctx0207c904 *ctx, Info0207c904 *info);
extern const unsigned char *func_0207c674(const unsigned char *p, int *out);
extern const unsigned char *func_0207c6f8(const unsigned char *p, unsigned int *out);

int func_0207c904(Iter0207c904 *iter)
{
    int s2a;
    int s3b;
    int s3a;
    int s4a;
    unsigned int s5c;
    unsigned int s5b;
    int s5a;
    int s6b;
    int s6a;
    int s7b;
    int s7a;
    int s8c;
    int s8b;
    int s8a;
    unsigned int s9d;
    unsigned int s9c;
    int s9b;
    int s9a;
    int s10a;
    int s11b;
    int s11a;
    int s12b;
    unsigned int s12a;
    int s13a;
    int s15c;
    unsigned int s15b;
    Count0207c904 s15;
    int s16c;
    int s16b;
    int s16a;
    int s17d;
    int s17b;
    int s17c;
    int s17a;
    unsigned int s18c;
    int s18b;
    int s18a;
    int s19a;
    int offset;
    const unsigned char *p;

    for (;;) {
        p = iter->info.action;
        if (p == 0 || (*p & 0x80) != 0) {
            func_0207c7e0(func_0207ded8(&iter->ctx, &iter->info), &iter->info);
            if (iter->info.table == 0) {
                return 0xff;
            }
            func_0207df34(&iter->ctx, &iter->info);
            if (iter->info.action == 0) {
                continue;
            }
            break;
        }
        switch (*p & 0x1f) {
        case 2:
            p = func_0207c674(p + 1, &s2a);
            iter->info.action = p + 4;
            break;
        case 3:
            p = func_0207c674(p + 1, &s3a);
            p = func_0207c674(p, &s3b);
            iter->info.action = p + 4;
            break;
        case 4:
            p = func_0207c674(p + 1, &s4a);
            iter->info.action = p + 4;
            break;
        case 5:
            p = func_0207c674(p + 1, &s5a);
            p = func_0207c6f8(p, &s5b);
            p = func_0207c6f8(p, &s5c);
            iter->info.action = p + 4;
            break;
        case 6:
            p = func_0207c674(p + 1, &s6a);
            p = func_0207c674(p, &s6b);
            iter->info.action = p + 4;
            break;
        case 7:
            p = func_0207c674(p + 1, &s7a);
            p = func_0207c674(p, &s7b);
            iter->info.action = p + 4;
            break;
        case 8:
            p = func_0207c674(p + 1, &s8a);
            p = func_0207c674(p, &s8b);
            p = func_0207c674(p, &s8c);
            iter->info.action = p + 4;
            break;
        case 9:
            p = func_0207c674(p + 1, &s9a);
            p = func_0207c674(p, &s9b);
            p = func_0207c6f8(p, &s9c);
            p = func_0207c6f8(p, &s9d);
            iter->info.action = p + 4;
            break;
        case 10:
            p = func_0207c674(p + 1, &s10a);
            iter->info.action = p + 4;
            break;
        case 11:
            p = func_0207c674(p + 1, &s11a);
            p = func_0207c674(p, &s11b);
            iter->info.action = p + 4;
            break;
        case 12:
            p = func_0207c6f8(p + 5, &s12a);
            p = func_0207c674(p, &s12b);
            iter->info.action = p;
            break;
        case 13:
            p = func_0207c674(p + 1, &s13a);
            iter->info.action = p;
            break;
        case 15:
            p = func_0207c6f8(p + 1, &s15.count);
            p = func_0207c6f8(p, &s15b);
            p = func_0207c674(p, &s15c);
            iter->info.action = p + s15.count * 4;
            break;
        case 16:
            p = func_0207c674(p + 1, &s16a);
            p = func_0207c674(p, &s16b);
            p += 4;
            p = func_0207c674(p, &s16c);
            iter->info.action = p + 4;
            break;
        case 17:
            p = func_0207c674(p + 1, &s17a);
            p = func_0207c674(p, &s17b);
            p += 1;
            p = func_0207c674(p, &s17c);
            p = func_0207c674(p, &s17d);
            iter->info.action = p + 4;
            break;
        case 18:
            p = func_0207c674(p + 1, &s18a);
            p += 1;
            p = func_0207c674(p, &s18b);
            p = func_0207c6f8(p, &s18c);
            iter->info.action = p + 4;
            break;
        case 19:
            p = func_0207c674(p + 1, &s19a);
            iter->info.action = p;
            break;
        default:
            return 0xff;
        }
        break;
    }
    while ((*iter->info.action & 0x1f) == 1) {
        func_0207c674(iter->info.action + 1, &offset);
        iter->info.action += offset;
    }
    return *iter->info.action & 0x1f;
}
