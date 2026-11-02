struct Vec0200f7b0 {
    int x;
    int y;
    int z;
};

extern char data_020cf93c[];
extern signed char data_02083de0[];
extern signed char data_02083de4[];

extern void *func_02009900(void);
extern void func_0202fad4(void *object, void *data);
extern void func_0200ee54(unsigned char *object, int value);
extern void func_0200eee0(unsigned char *object);
extern void func_0200da04(signed char *name, signed char *value, signed char *text, signed char *extra);
extern void func_0200f8ec(struct Vec0200f7b0 *v, int x, int y, int z);
extern int func_0206f114(struct Vec0200f7b0 *v);
extern int func_0206ef28(int a, int b);
extern int func_0200ee64(int value);
extern void func_0206f174(struct Vec0200f7b0 *src, struct Vec0200f7b0 *dst);
extern int func_0206f28c(int a, int b);
extern void *func_0205a654(void *data);
extern void func_0200f8f4(void *shot, int x, int y, int dx, int dy, int length);
extern void func_0200f908(void *shot, signed char *value, signed char *text);
extern int func_0200f330(void *object);
extern void func_0200f910(void *shot, int visible);
extern void func_0202f7e8(void *list, void *shot, int a, int b);

void func_0200f7b0(unsigned char *object, int x, int y, int scale, signed char *value, signed char *text)
{
    struct Vec0200f7b0 dir;
    int length;
    int dx;
    int dy;
    void *shot;
    int visible;
    int limit;

    func_0202fad4(func_02009900(), data_020cf93c);
    func_0200ee54(object, 0);
    func_0200eee0(object);
    if (scale >= 0x3e8000) {
        *(int *)(object + 0xd94) = x;
        *(int *)(object + 0xd98) = y;
        *(unsigned int *)(object + 8) |= 0x80;
        func_0200da04(data_02083de0, value, text, data_02083de4);
        return;
    }
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    limit = *(unsigned short *)(object + 0xdc8) - 0x100;
    if ((x >> 12) > limit) {
        x = limit << 12;
    }
    limit = *(unsigned short *)(object + 0xdca) - 0xc0;
    if ((y >> 12) > limit) {
        y = limit << 12;
    }
    if (scale <= 0) {
        scale = 0x1000;
    }
    func_0200f8ec(&dir, x - *(int *)(object + 0xd9c), *(int *)(object + 0xda0) - y, 0);
    length = func_0200ee64(func_0206ef28(func_0206f114(&dir), scale));
    func_0206f174(&dir, &dir);
    dx = func_0206f28c(dir.x, scale);
    dy = -func_0206f28c(dir.y, scale);
    shot = func_0205a654(data_020cf93c);
    if (shot != 0) {
        func_0200ee54(object, 1);
        func_0200f8f4(shot, x, y, dx, dy, length);
        func_0200f908(shot, value, text);
        visible = func_0200f330(object) != 0;
        func_0200f910(shot, visible);
        func_0202f7e8(func_02009900(), shot, 0xd, 0);
    }
}
