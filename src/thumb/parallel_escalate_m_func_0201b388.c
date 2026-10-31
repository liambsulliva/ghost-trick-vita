struct Window {
    unsigned char pad0[0x14];
    unsigned int f14;
    unsigned int f18;
    unsigned char pad1c[0x22 - 0x1c];
    unsigned char f22;
    unsigned char pad23[0x46 - 0x23];
    unsigned short f46;
    unsigned short f48;
    unsigned short f4a;
    unsigned short f4c;
    unsigned char pad4e[0x74 - 0x4e];
    unsigned short width;
    unsigned char height;
    unsigned char pad77;
    unsigned int area;
    unsigned char pad7c[0x8c - 0x7c];
    unsigned short f8c;
};
extern void func_0201b7c0(struct Window *win);
extern void func_0201bcac(struct Window *win, unsigned int value);
extern void func_0201b890(struct Window *win);

void func_0201b388(struct Window *win, unsigned int value, unsigned int w, unsigned int h)
{
    win->f8c = 0xf;
    func_0201b7c0(win);
    func_0201bcac(win, win->f22);
    win->f18 = value;
    win->f14 = value;
    if (w != 0 && w <= 0x100 && h != 0 && h <= 0xc0) {
        win->width = (int)(w * 0x20) / 256;
        win->height = (int)(h * 0x18) / 0xc0;
        win->f46 = win->width << 3;
        win->f48 = win->height << 3;
        win->f4a = win->f46;
        win->f4c = win->f46;
        win->area = win->width * win->height * 0x20 + 0x20;
    }
    func_0201b890(win);
}
