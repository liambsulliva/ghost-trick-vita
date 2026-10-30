class Reader {
public:
    virtual void m0();
    virtual void m4();
    virtual int open(unsigned int id, int mode, unsigned int bank, int flags);
};

struct Loader {
    Reader *reader;
    int pad4;
    int pad8;
    int handle;
    int pad10;
    int kind;
    unsigned short id;
    unsigned short bank;
};

extern "C" {
int func_0201c7c8(struct Loader *loader);
int func_0201cac8(struct Loader *loader);

int func_0201c710(struct Loader *loader)
{
    if (loader->id > 0xff00) {
        return 4;
    }
    loader->handle = loader->reader->open(loader->id, 2, (unsigned char)loader->bank, 0);
    if (loader->handle == 0) {
        return 5;
    }
    switch (loader->kind) {
    case 2:
        return func_0201c7c8(loader);
    case 3:
        return func_0201cac8(loader);
    }
    return 3;
}
}
