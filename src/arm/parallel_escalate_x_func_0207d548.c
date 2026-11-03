typedef struct Spec0207d548 {
    unsigned int count;
    unsigned int pcoffset;
    int cinfo;
    const unsigned char *list;
} Spec0207d548;

extern int func_0207da40(unsigned int a, unsigned int type, void *thrown);

int func_0207d548(unsigned int a, Spec0207d548 *spec)
{
    const unsigned char *p;
    unsigned int i;
    unsigned int type;
    char thrown[8];

    p = spec->list;
    for (i = 0; i < spec->count; i++) {
        type = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
        if (func_0207da40(a, type, thrown) != 0) {
            return 1;
        }
        p += 4;
    }
    return 0;
}
