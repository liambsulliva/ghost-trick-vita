class Stream0200e410 {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18(unsigned int pos, int whence);
    virtual void v1c(void *dst, int size);
};

struct Pair0200e410 {
    unsigned char lo;
    unsigned char hi;
};

struct File0200e410 {
    unsigned char *data;
    unsigned short size;
    unsigned char pad6;
    unsigned char memory;
    unsigned char open;
    unsigned char pad9[3];
    Stream0200e410 stream;
};

extern "C" {
extern void func_0207c594(void *dst, int size);
extern void func_0207303c(void *src, void *dst, int size);

Pair0200e410 func_0200e410(File0200e410 *param_1, unsigned int param_2)
{
    Pair0200e410 buf;
    unsigned int pos;

    func_0207c594(&buf, 2);
    if (param_1->open == 0) {
    return buf;
    }
    pos = param_2 * 2;
    if (pos < param_1->size) {
        if (param_1->memory == 0) {
            param_1->stream.v18(pos, 0);
            param_1->stream.v1c(&buf, 2);
        } else {
            func_0207303c(param_1->data + pos, &buf, 2);
        }
    }
    return buf;
}
}
