typedef struct {
    unsigned int first;
    unsigned int last;
    unsigned short count;
    unsigned short offset;
} List02022bc0;

typedef struct {
    int pad0;
    int align;
    int pad8;
    int padc;
    List02022bc0 used;
    List02022bc0 free;
    List02022bc0 spare;
} Alloc02022bc0;

typedef struct {
    unsigned char pad[0xf08];
    Alloc02022bc0 first;
    Alloc02022bc0 second;
} Pool02022bc0;

extern int func_020225e0(Alloc02022bc0 *a, int size, unsigned char tag);

int func_02022bc0(Pool02022bc0 *a, int size, int flag)
{
    int r;

    if (size == 0) {
        return -1;
    }
    if (size >= 0x4000) {
        size = -size;
    }
    if (flag != 0) {
        r = func_020225e0(&a->first, size, 0);
        if (r != -1) {
            return r;
        }
    }
    r = func_020225e0(&a->second, size, 0);
    if (r == -1) {
        return -1;
    }
    return r + 0x6000;
}
