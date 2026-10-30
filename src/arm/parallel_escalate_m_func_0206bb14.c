typedef struct FSArchive {
    unsigned int name;
    struct FSArchive *next;
    struct FSArchive *prev;
    char pad[8];
    unsigned int flag;
} FSArchive;

extern FSArchive *data_020d0bac;
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);
extern const signed char *func_0206c0e0(void *arc);
extern int func_02079180(const signed char *a, const signed char *b, int n);

static inline int is_loaded(volatile const FSArchive *arc)
{
    return (arc->flag & 2) ? 1 : 0;
}

FSArchive *func_0206bb14(const signed char *name, int name_len)
{
    unsigned int bak;
    FSArchive *p;

    bak = func_02075c98();
    for (p = data_020d0bac; p != 0; p = p->next) {
        if (is_loaded(p)) {
            const signed char *arc_name = func_0206c0e0(p);
            if (func_02079180(arc_name, name, name_len) == 0 && arc_name[name_len] == 0) {
                break;
            }
        }
    }
    func_02075cac(bak);
    return p;
}
