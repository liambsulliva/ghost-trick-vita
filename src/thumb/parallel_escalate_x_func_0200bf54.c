struct OamAffine {
    unsigned int y : 8;
    unsigned int affine : 2;
    unsigned int objmode : 2;
    unsigned int mosaic : 1;
    unsigned int color : 1;
    unsigned int shape : 2;
    unsigned int x : 9;
    unsigned int matrix : 5;
    unsigned int size : 2;
    unsigned int tile : 10;
    unsigned int priority : 2;
    unsigned int palette : 4;
    unsigned int pad : 16;
};

union Oam {
    unsigned short attr[4];
    struct OamAffine a;
    unsigned int w[2];
};

struct Sprites {
    union Oam oam[0x140];
    unsigned char lists[0x80];
    unsigned char count;
    unsigned char pada81;
    unsigned char affine_count;
};

struct Node {
    unsigned char pad0[0x11];
    unsigned char first;
    unsigned char drawn;
    unsigned char pad13;
    unsigned short frame;
    unsigned char pad16[6];
    unsigned char flip : 2;
    unsigned char pad1c : 6;
    unsigned char scale;
    unsigned char affine : 2;
    unsigned char objmode : 2;
    unsigned char mosaic : 1;
    unsigned char priority : 3;
    unsigned char pad1f;
    int sin;
    int cos;
    int mtx[4];
    unsigned char pad38[4];
    unsigned char *ref;
};

extern int data_027e0060;
extern int data_027e0064;
extern void *func_0200b848(struct Node *node);
extern unsigned short *func_0200b854(struct Node *node);
extern int func_0200c2c0(struct Sprites *p, struct Node *node, unsigned short *src);
extern void func_0200857c(unsigned char *ref);
extern unsigned int func_0200c1d4(unsigned char *ref);
extern unsigned int func_0200c1dc(unsigned char *ref);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern void func_0200c1e4(int *mtx, unsigned int flip);
extern int func_0200c470(struct Sprites *p, int *mtx);
extern int func_0200b0f4(void *cell);
extern unsigned char func_0200b114(void *cell);
extern unsigned char func_0200b794(void *cell);
extern int func_0200b774(void *cell);
extern void func_0200b074(void *cell);
extern void func_02008560(unsigned char *ref);
extern void func_02008a98(unsigned char *ref);

void func_0200bf54(struct Sprites *p, struct Node *node)
{
    int mtx[4];
    void *cell;
    unsigned short *src;
    int n;
    unsigned short *anim;
    unsigned short *data;
    unsigned int count;
    union Oam *oam;

    if (p->count >= 0x80) {
        return;
    }
    cell = func_0200b848(node);
    if (cell == 0) {
        return;
    }
    anim = func_0200b854(node);
    if (anim == 0) {
        return;
    }
    if (anim[0] < node->frame) {
        return;
    }
    data = anim + anim[node->frame];
    count = p->count;
    src = data + 1;
    n = data[0];
    oam = &p->oam[count];
    node->drawn = 0;
    node->first = count;
    for (; n-- > 0 && count < 0x80; src += 3) {
        if (func_0200c2c0(p, node, src) == 0) {
            continue;
        }
        if (node->ref != 0) {
            func_0200857c(node->ref);
            if (func_0200c1d4(node->ref) == 0) {
                break;
            }
        }
        oam->attr[0] = src[0];
        oam->attr[1] = src[1];
        oam->attr[2] = src[2];
        if (node->affine != 0 && p->affine_count < 0x20) {
            func_0207303c(node->mtx, mtx, 0x10);
            if (node->flip != 0) {
                func_0200c1e4(mtx, node->flip);
            }
            oam->a.affine = node->affine;
            oam->a.matrix = func_0200c470(p, mtx);
        } else if (node->flip != 0) {
            oam->w[0] ^= (node->flip << 28) & 0x30000000;
        }
        if (node->scale != 0 && node->scale != 1) {
            oam->a.tile = oam->a.tile * node->scale;
        }
        oam->a.y = data_027e0064;
        oam->a.x = data_027e0060;
        oam->a.tile += func_0200b0f4(cell);
        oam->a.palette += func_0200b114(cell);
        oam->a.priority = node->priority;
        oam->a.objmode = node->objmode;
        oam->a.mosaic = node->mosaic;
        oam->a.color = func_0200b794(cell);
        oam++;
        count++;
        node->drawn++;
    }
    if (node->ref != 0 && p->count == count && func_0200c1dc(node->ref) == 0) {
        func_02008560(node->ref);
        func_02008a98(node->ref);
    }
    if (func_0200b774(cell) != 0 && p->count == count) {
        func_0200b074(cell);
    }
    p->count = count;
}
