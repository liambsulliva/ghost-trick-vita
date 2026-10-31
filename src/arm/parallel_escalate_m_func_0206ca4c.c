typedef struct {
    int n;
    int x;
} Pair;

struct Entry {
    int w0;
    unsigned int w4;
};

struct File {
    unsigned int w[18];
};

extern void func_0206c440(struct File *file);
extern Pair func_0206c8c0(struct Entry *entry);
extern int func_0206c538(struct File *file, Pair pair);
extern unsigned int func_0206c868(struct Entry *entry);
extern void func_0206c884(struct Entry *entry);
extern unsigned int func_0206c704(struct File *file, unsigned int dst, unsigned int len);
extern void func_0206c614(struct File *file);

int func_0206ca4c(struct Entry *entry)
{
    int ret;
    unsigned int len;
    Pair pair;
    struct File file;

    ret = 0;
    func_0206c440(&file);
    pair = func_0206c8c0(entry);
    if (func_0206c538(&file, pair)) {
        len = func_0206c868(entry);
        func_0206c884(entry);
        if (len == func_0206c704(&file, entry->w4, len)) {
            ret = 1;
        }
        func_0206c614(&file);
    }
    return ret;
}
