typedef struct MWiDestructorChain
{
    struct MWiDestructorChain *next;
    void (*dtor)(void *);
    void *obj;
} MWiDestructorChain;

typedef struct
{
    unsigned int id;
    unsigned char *ram_address;
    unsigned int ram_size;
    unsigned int bss_size;
} FSOverlayInfo;

extern MWiDestructorChain *data_020d3a3c;
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);

void func_0206cc7c(FSOverlayInfo *p_ovi)
{
    for (;;) {
        MWiDestructorChain *head = 0;
        MWiDestructorChain *tail = 0;
        unsigned int region_top = (unsigned int)p_ovi->ram_address;
        unsigned int region_bottom = region_top + (p_ovi->ram_size + p_ovi->bss_size);
        unsigned int bak_psr;
        MWiDestructorChain *prev;
        MWiDestructorChain *base;
        MWiDestructorChain *p;

        bak_psr = func_02075c98();
        prev = 0;
        base = data_020d3a3c;
        p = base;

        while (p) {
            MWiDestructorChain *next = p->next;
            const unsigned int dtor = (unsigned int)p->dtor;
            const unsigned int obj = (unsigned int)p->obj;
            if (((obj == 0) && (dtor >= region_top) && (dtor < region_bottom)) ||
                ((obj >= region_top) && (obj < region_bottom))) {
                if (!tail) {
                    head = p;
                } else {
                    tail->next = p;
                }
                if (base == p) {
                    base = data_020d3a3c = next;
                }
                p->next = 0;
                tail = p;
                if (prev) {
                    prev->next = next;
                }
            } else {
                prev = p;
            }
            p = next;
        }
        func_02075cac(bak_psr);

        if (!head) {
            return;
        }
        do {
            MWiDestructorChain *next = head->next;
            if (head->dtor) {
                (*head->dtor)(head->obj);
            }
            head = next;
        } while (head);
    }
}
