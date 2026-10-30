extern char data_020abca4;
extern char *func_02050a44(int size);
extern char *func_0205083c(char *node);
extern void func_02050aec(char *part, int value, int a, int b);
extern void func_02050fd4(char *list, char *node, int a, int b);
extern int func_0200d84c(char *object, void *data);

void func_020511c0(char *self, int a, int b)
{
    char *node;
    char *last;
    node = func_02050a44(0x5c);
    if (node != 0) {
        node = func_0205083c(node);
    }
    if (node != 0) {
        func_02050aec(node + 0x14, *(int *)(self + 0x34), a, b + 6);
        *(char **)(node + 0x50) = self;
        func_02050fd4(self + 4, node, a, b + 4);
        if (func_0200d84c(node + 4, &data_020abca4) != 0) {
            *(char **)(self + 0x44) = node;
        }
        last = *(char **)(self + 0x3c);
        if (last != 0) {
            while (*(char **)(last + 0x54) != 0) {
                last = *(char **)(last + 0x54);
            }
            *(char **)(last + 0x54) = node;
            return;
        }
        *(char **)(node + 0x54) = 0;
        *(char **)(self + 0x3c) = node;
    }
}
