extern void *func_02008444(void);
extern int func_0200f5c8(void *cam);
extern int func_0200f5e8(void *cam);
extern int func_02010194(void *cam);
extern void func_02058a80(int *dst, int src);
extern void func_020570cc(void *obj);

typedef struct {
    char pad00[0x28];
    int x;
    int y;
    char pad30[0x58];
    int sx;
    int sy;
    char pad90[9];
    unsigned char mode;
} Obj;

void func_02062468(Obj *obj)
{
    int pos[3];

    obj->sx = obj->x;
    obj->sy = obj->y;
    if (obj->mode == 1) {
        obj->sx = obj->x - 0x80000;
        obj->sy = -obj->y + 0x60000;
    } else if (obj->mode == 2) {
        func_02058a80(pos, func_02010194(func_02008444()));
        obj->sx = pos[0] - func_0200f5c8(func_02008444()) - 0x80000;
        obj->sy = -pos[1] + func_0200f5e8(func_02008444()) + 0x60000;
    } else {
        obj->sx = obj->x - func_0200f5c8(func_02008444()) - 0x80000;
        obj->sy = -obj->y + func_0200f5e8(func_02008444()) + 0x60000;
    }
    func_020570cc(obj);
}
