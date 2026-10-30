extern void func_0202b074(char *object, int *x, int *y, int third, int fourth);
extern int func_0202a890(char *object, int kind, unsigned short x, short y, int zero);
extern void func_02061c08(int result, int third, int fourth);

int func_0202a114(char *object, int kind, int third, int fourth)
{
    int x;
    int y;
    int result;

    if (*(unsigned char *)(object + 0x24) == 1) {
        return 0;
    }
    if (kind == 0 || kind == 0x21) {
        return 0;
    }
    func_0202b074(object, &x, &y, third, fourth);
    result = func_0202a890(object, kind, x, y, 0);
    if (result != 0) {
        func_02061c08(result, third, fourth);
    }
    return result;
}
