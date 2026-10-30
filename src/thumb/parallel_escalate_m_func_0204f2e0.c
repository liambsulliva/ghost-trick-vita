extern unsigned char *func_02009c1c();
extern int func_0200acc8();
extern void func_0200ad10(int param_1, unsigned short param_2);
extern unsigned short *func_0200eb78();
extern int func_0201c0f8();
extern int func_02022c64();
extern int func_02029024();
extern int func_0202f1bc();
void func_0204f2e0(int param_1)
{
    int index;
    int sprite;
    int frame;
    int handle;
    index = *(signed char *)(param_1 + 0xe3);
    if (index < 0) {
        index = func_0200acc8(param_1 + 0xe4);
    }
    sprite = func_0201c0f8(*(unsigned char **)(param_1 + 0x48));
    frame = func_02029024(*(unsigned char **)(param_1 + 0x48));
    handle = func_02022c64(func_0200eb78(), frame);
    if (*(unsigned char *)(param_1 + 0xe2) != 0) {
        func_0200ad10(sprite, (unsigned short)(index << 5));
    } else {
        func_0200ad10(sprite, (unsigned short)index);
    }
    func_0202f1bc(func_02009c1c(), sprite, frame, handle, 2);
}
