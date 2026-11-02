typedef struct Frame02069318 {
    void *content;
    unsigned short frames;
    unsigned short pad;
} Frame02069318;

typedef struct Seq02069318 {
    unsigned short count;
    unsigned short loop_start;
    unsigned int type;
    unsigned int mode;
    Frame02069318 *frames;
} Seq02069318;

typedef struct SeqAttr02069318 {
    unsigned short count;
    unsigned short pad;
    unsigned int attr;
    unsigned int frame_attr;
} SeqAttr02069318;

typedef struct AttrBank02069318 {
    unsigned short count;
    unsigned short attrs;
    SeqAttr02069318 *seqs;
} AttrBank02069318;

typedef struct Block02069318 {
    unsigned int id;
    unsigned int size;
    AttrBank02069318 bank;
} Block02069318;

typedef struct Anim02069318 {
    unsigned short count;
    unsigned short total;
    Seq02069318 *seqs;
    Frame02069318 *frames;
    unsigned int elems;
    unsigned int strings;
    Block02069318 *ext;
} Anim02069318;

void func_02069318(Anim02069318 *p)
{
    unsigned short i;
    unsigned short j;
    Seq02069318 *seqs;
    unsigned int frames;
    unsigned int elems;
    AttrBank02069318 *bank;
    unsigned int k;
    unsigned int m;
    SeqAttr02069318 *g;

    seqs = (Seq02069318 *)((unsigned int)p->seqs + (unsigned int)p);
    p->seqs = seqs;
    frames = (unsigned int)p->frames + (unsigned int)p;
    p->frames = (Frame02069318 *)frames;
    elems = p->elems + (unsigned int)p;
    p->elems = elems;
    for (i = 0; i < p->count; i++) {
        seqs[i].frames = (Frame02069318 *)((unsigned int)seqs[i].frames + frames);
        for (j = 0; j < seqs[i].count; j++) {
            seqs[i].frames[j].content = (void *)((unsigned int)seqs[i].frames[j].content + elems);
        }
    }
    if (p->ext != 0) {
        p->ext = (Block02069318 *)((unsigned int)p->ext + (unsigned int)p);
        bank = &p->ext->bank;
        bank->seqs = (SeqAttr02069318 *)((unsigned int)bank->seqs + (unsigned int)bank);
        for (k = 0; k < bank->count; k++) {
            g = &bank->seqs[k];
            g->attr = g->attr + (unsigned int)bank;
            g->frame_attr = g->frame_attr + (unsigned int)bank;
            for (m = 0; m < g->count; m++) {
                ((unsigned int *)g->frame_attr)[m] = ((unsigned int *)g->frame_attr)[m] + (unsigned int)bank;
            }
        }
    }
}
