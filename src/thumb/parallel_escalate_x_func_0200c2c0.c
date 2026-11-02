typedef struct Attr0200c2c0 {
    unsigned short attr[3];
} Attr0200c2c0;

typedef struct Sprite0200c2c0 {
    char pad[0x16];
    short x;
    short y;
    char pad1a[2];
    unsigned char flip : 2;
    unsigned char pad1c : 6;
    char pad1d;
    unsigned char affine : 2;
    unsigned char pad1e : 6;
    char pad1f;
    int sin;
    int cos;
} Sprite0200c2c0;

extern int data_027e0060;
extern int data_027e0064;
extern int func_0200c3fc(Attr0200c2c0 *attr);
extern int func_0200c40c(Attr0200c2c0 *attr);
extern unsigned int func_0200c41c(Attr0200c2c0 *attr);
extern int func_0200c428(unsigned int *size);
extern int func_0200c44c(unsigned int *size);
extern void func_0200b3fc(int *mtx, int sin, int cos);
extern void func_0200c220(const int *m, const int *v, int *dst);
extern int func_0206f28c(int a, int b);

int func_0200c2c0(void *unused, Sprite0200c2c0 *sprite, Attr0200c2c0 *src)
{
    Attr0200c2c0 attr;
    int pos[2];
    unsigned int size;
    int mtx[4];
    int w;
    int h;
    int flip;
    int x;
    int y;

    attr = *src;
    pos[0] = func_0200c3fc(&attr) << 12;
    pos[1] = func_0200c40c(&attr) << 12;
    size = func_0200c41c(&attr);
    w = func_0200c428(&size) << 12;
    h = func_0200c44c(&size) << 12;
    flip = sprite->flip;
    if (flip != 0 || sprite->affine != 0) {
        if (flip != 0) {
            if (flip & 1) {
                pos[0] = -(pos[0] + w);
            }
            if (sprite->flip & 2) {
                pos[1] = -(pos[1] + h);
            }
        }
        if (sprite->affine != 0) {
            func_0200b3fc(mtx, sprite->sin, sprite->cos);
            func_0200c220(mtx, pos, pos);
            w >>= 1;
            h >>= 1;
            pos[0] += -w + func_0206f28c(mtx[0], w) + func_0206f28c(mtx[2], h);
            pos[1] += -h + func_0206f28c(mtx[1], w) + func_0206f28c(mtx[3], h);
            if (sprite->affine == 3) {
                pos[0] -= w;
                pos[1] -= h;
            }
        }
    }
    pos[0] += sprite->x << 12;
    pos[1] += sprite->y << 12;
    x = pos[0] >> 12;
    data_027e0060 = x;
    y = pos[1] >> 12;
    data_027e0064 = y;
    if (-(w >> 12) < x && x < 0x100 && -(h >> 12) < y && y < 0xc0) {
        return 1;
    }
    return 0;
}
