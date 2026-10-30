extern char data_02083dc0[];
extern char data_02083de0[];
extern char data_02083de4[];
extern char data_02083dec[];
extern char data_02083dfc[];
extern int func_0200fa54(unsigned char *object);
extern void func_0200f7b0(unsigned char *object, int x, int y, int value, char *a, char *b);
extern int func_02012518(unsigned char *object, char *a, char *b);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern void func_0200f958(unsigned char *object, int value);

void func_0200f9e8(unsigned char *object, int value)
{
    if (func_0200fa54(object) != 0) {
        func_0200f7b0(object, *(int *)(object + 0xdac), *(int *)(object + 0xdb0), value, data_02083dc0, data_02083dec);
        return;
    }
    if (func_02012518(object, data_02083dc0, data_02083dfc) != 0) {
        func_0200da04(data_02083de0, data_02083dc0, data_02083dfc, data_02083de4);
        return;
    }
    func_0200f958(object, value);
}
