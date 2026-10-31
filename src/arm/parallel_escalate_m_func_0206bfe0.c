typedef struct Archive {
    char *name;
    struct Archive *next;
    int pad[3];
    unsigned int flag;
} Archive;

extern Archive *data_020d0bac;
extern signed char data_020d0bbc[16][16];
extern unsigned int func_02075c98(void);
extern void func_02075cac(unsigned int intr);
extern Archive *func_0206bb14(const char *name, unsigned int len);
extern int func_02078eb8(void *dst, const char *src, unsigned int len);
extern void func_02075f84(void);

int func_0206bfe0(Archive *arc, const char *name, unsigned int len)
{
    int ret = 0;
    unsigned int intr = func_02075c98();

    if (!func_0206bb14(name, len)) {
        Archive **pp;
        Archive *cur;
        pp = (Archive **)(data_020d0bbc - 1);
        for (cur = data_020d0bac; cur; cur = cur->next) {
            pp = &cur->next;
        }
        *pp = arc;
        if (len <= 3) {
            arc->name = 0;
            func_02078eb8(arc, name, len + 1);
        } else if (len <= 15) {
            int i;
            for (i = 0; ; ++i) {
                if (i >= 16) {
                    func_02075f84();
                } else if (data_020d0bbc[i][0] == 0) {
                    func_02078eb8(data_020d0bbc[i], name, len + 1);
                    arc->name = (char *)data_020d0bbc[i];
                    break;
                }
            }
        } else {
            func_02075f84();
        }
        arc->flag |= 1;
        ret = 1;
    }
    func_02075cac(intr);
    return ret;
}
