class Stream0200a6f0 {
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

class Obj0200a6f0 {
public:
    virtual ~Obj0200a6f0();
};

struct Head0200a6f0 {
    unsigned short first;
    unsigned short count : 15;
    unsigned short flag : 1;
};

struct File0200a6f0 {
    Stream0200a6f0 *stream;
    int pad4;
    unsigned short h8;
    unsigned short ha;
    int size;
};

extern "C" {
extern void *func_0200883c(void);
extern void *func_02008824(void *object);
extern Stream0200a6f0 *func_0201e664(void *object, int mode, unsigned int id);
extern void *func_02031254(int size);
extern void func_02031270(void *ptr);
extern unsigned short *func_0200a780(File0200a6f0 *file);
extern Obj0200a6f0 *func_0200a378(void *object);
extern void func_0200d114(Obj0200a6f0 *dst, unsigned short *src, unsigned short *data, int key, int arg);

void func_0200a6f0(File0200a6f0 *param_1, unsigned int param_2)
{
    Head0200a6f0 *buf;
    unsigned short *data;
    Obj0200a6f0 *obj;

    param_1->stream = func_0201e664(func_02008824(func_0200883c()), 4, param_2);
    if (param_1->stream != 0) {
        buf = (Head0200a6f0 *)func_02031254(0x200);
        if (buf != 0) {
            param_1->stream->v1c(buf, 4);
            param_1->h8 = buf->first;
            param_1->ha = buf->count;
        }
        data = func_0200a780(param_1);
        if (data != 0 && param_1->size != 0x1000) {
            obj = (Obj0200a6f0 *)func_02031254(4);
            if (obj != 0) {
                obj = func_0200a378(obj);
            }
            if (obj != 0) {
                func_0200d114(obj, data, data, 0x200, param_1->size >> 4);
                delete obj;
            }
        }
        func_02031270(buf);
    }
}
}
