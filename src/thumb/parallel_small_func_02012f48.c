extern void func_0202fbb0(char *iterator);
extern int func_0200eac4(char *iterator);
extern char *func_0200eab4(char *iterator);
extern int func_02008828(char *object, char *kind);
extern int func_0200eab8(char *iterator);
extern void func_0200eabc(char *iterator, int index);

int func_02012f48(int param_1, char *param_2, int param_3)
{
    char iterator[16];
    int found;

    func_0202fbb0(iterator);
    found = 0;
    while (func_0200eac4(iterator) != 0) {
        if (func_02008828(func_0200eab4(iterator), param_2) != 0) {
            if (found == param_3) {
                return func_0200eab8(iterator);
            }
            found = found + 1;
        }
        func_0200eabc(iterator, 0);
    }
    return 0;
}
