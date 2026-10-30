struct Shape {
    int word0;
    int index;
    int word8;
    int mode;
};
struct Entry {
    unsigned char kind;
    unsigned char rest[15];
};
extern struct Entry data_02083c84[];
extern unsigned int func_0200e248(struct Shape *shape);
extern unsigned int func_0200e26c(struct Shape *shape);
extern void func_0200e1cc(struct Shape *shape,char *src,char *dst,unsigned int value);
extern void func_0200e1c4(char *src,char *dst,unsigned int size);
extern void func_0200e214(struct Shape *shape,char *src,char *dst,unsigned int size);

void func_0200e12c(struct Shape *shape,char *src,char *dst)
{
    unsigned int value;
    unsigned int size;
    unsigned char kind;

    if (src != 0 && dst != 0 && shape->index < 3) {
        if (shape->mode == 2) {
            if (data_02083c84[shape->index].kind == 1) {
                func_0200e1cc(shape,src,dst,func_0200e248(shape));
            }
            else {
                func_0200e1c4(src,dst,func_0200e26c(shape));
            }
        }
        else if (shape->mode == 3) {
            value = func_0200e248(shape);
            size = func_0200e26c(shape);
            kind = data_02083c84[shape->index].kind;
            if (kind == 1) {
                func_0200e1cc(shape,src,dst + size,value);
                func_0200e214(shape,dst + size,dst,size);
            }
            else if (kind == 2) {
                func_0200e214(shape,src,dst,size);
            }
        }
    }
}
