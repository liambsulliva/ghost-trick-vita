struct Entry {
    unsigned short h0;
    unsigned short h2;
    unsigned short h4;
    unsigned short h6;
    unsigned short h8;
    unsigned short ha;
    unsigned short hc;
};

struct Voice {
    unsigned char pad0[0x10];
    unsigned char b10;
    unsigned char pad11[0x27];
    unsigned short h38;
    unsigned short h3a;
    unsigned short h3c;
    unsigned char pad3e;
    unsigned char b3f;
    unsigned char b40;
    unsigned char pad41;
    unsigned short h42;
    unsigned short h44;
    unsigned short h46;
    unsigned short h48;
    unsigned char b4a;
    unsigned char pad4b;
    unsigned short h4c;
    unsigned char pad4e[0x16];
    int w64;
    int w68;
    int w6c;
};

extern struct Entry data_02085724[];

void func_020541d8(struct Voice *v, int index, int a, int b)
{
    struct Entry *e;

    if (v->b10 != 0) {
        v->b40 = 1;
    }
    e = &data_02085724[index];
    v->h42 = e->h0;
    v->h44 = e->h4 & 1;
    v->b3f = a != 0 ? 1 : 0;
    v->h38 = b != 0 ? 0 : e->h2;
    v->h3a = e->h8;
    v->h3c = e->ha;
    v->h46 = 0;
    v->b4a = 0;
    v->h4c = b != 0 ? 0 : e->h2;
    v->w64 = 0;
    v->w68 = 0x333;
    v->w6c = 0;
}
