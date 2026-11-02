typedef struct {
    unsigned int y : 8;
    unsigned int mode : 8;
    unsigned int x : 9;
    unsigned int shape : 7;
    unsigned int tile : 12;
    unsigned int palette : 4;
    unsigned int pad : 16;
} Oam;

struct Node {
    unsigned char pad0[0x12];
    unsigned char count;
    unsigned char pad13[0x9];
    unsigned char low : 3;
    unsigned char shadow : 1;
    unsigned char palette : 4;
    unsigned char pad1d[0x1b];
    struct Node *next;
};

struct List {
    struct Node *head;
    struct Node *tail;
};

struct Sprites {
    Oam oam[0x140];
    struct List lists[16];
    unsigned char count;
    unsigned char pada81[0x3];
    unsigned char layer;
};

extern void func_0200b760(struct Node *node, int layer);
extern void func_0200bf54(struct Sprites *p, struct Node *node);
extern void func_0207303c(const void *src, void *dst, unsigned int size);

void func_0200bdcc(struct Sprites *p)
{
    Oam tmp;
    unsigned int i;
    Oam *src;
    unsigned int j;
    unsigned int k;
    struct Node *node;
    Oam *dst;

    for (i = 0; i < 16; i++) {
        for (node = p->lists[i].head; node != 0; node = node->next) {
            func_0200b760(node, p->layer);
            func_0200bf54(p, node);
            if (node->shadow) {
                src = &p->oam[p->count - node->count];
                dst = &p->oam[p->count];
                for (j = 0; j < node->count && p->count < 0x80; j++) {
                    func_0207303c(&src[j], &tmp, 8);
                    tmp.x = tmp.x - 1;
                    tmp.y = tmp.y - 1;
                    tmp.palette = node->palette;
                    func_0207303c(&tmp, dst, 8);
                    p->count++;
                    dst++;
                }
                for (k = 0; k < node->count && p->count < 0x80; k++) {
                    func_0207303c(&src[k], &tmp, 8);
                    tmp.x = tmp.x + 1;
                    tmp.y = tmp.y + 1;
                    tmp.palette = node->palette;
                    func_0207303c(&tmp, dst, 8);
                    p->count++;
                    dst++;
                }
            }
        }
    }
}
