typedef struct {
    unsigned long long size;
    int unk8;
    int unkc;
    int min_x;
    int unk14;
    int min_y;
    int unk1c;
    unsigned long long unk20;
    unsigned long long unk28;
} Info0206df24;

extern unsigned int func_02069ba8(void);

int func_0206df24(void *a, Info0206df24 *obj)
{
    char *base;

    base = (char *)func_02069ba8();
    obj->unk20 = 0;
    obj->unk28 = 0;
    obj->size = *(unsigned int *)(base + 0x80);
    obj->unk8 = 0;
    obj->unkc = 0;
    obj->min_x = 0x7fffffff;
    obj->unk14 = 0;
    obj->min_y = 0x7fffffff;
    obj->unk1c = 0;
    return 0;
}
