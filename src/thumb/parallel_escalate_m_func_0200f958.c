struct Vec0200f958 {
    int x;
    int y;
};

struct Flags0200f958 {
    unsigned int low : 12;
    unsigned int locked : 1;
};

extern char data_020cf93c[];

extern void *func_02009900(void);
extern void func_0202fad4(void *object, void *data);
extern void func_0200ee54(unsigned char *object, int value);
extern int func_02010194(unsigned char *object);
extern void func_02058a80(struct Vec0200f958 *dst, int src);
extern int func_02012318(unsigned char *object);
extern void func_0200f9cc(struct Vec0200f958 *pos, unsigned char *object);
extern void func_0200f61c(unsigned char *object, int x, int y, int arg);

void func_0200f958(unsigned char *object, int arg)
{
    struct Vec0200f958 pos;
    int source;

    func_0202fad4(func_02009900(), data_020cf93c);
    func_0200ee54(object, 0);
    source = func_02010194(object);
    if (source != 0) {
        func_02058a80(&pos, source);
        pos.x -= 0x80000;
        if (((struct Flags0200f958 *)(object + 0xc))->locked) {
            pos.y -= 0x60000;
        } else {
            pos.y = func_02012318(object + 0xde4);
        }
        func_0200f9cc(&pos, object + 0xdd8);
        func_0200f61c(object, pos.x, pos.y, arg);
    }
}
