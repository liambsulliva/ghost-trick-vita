extern char data_020abc94;
extern void func_0200d824(unsigned int *buffer, void *key);
extern int func_0200d84c(unsigned int *buffer, void *other);
extern void func_0200db44(unsigned int *buffer);
extern int func_020509b8(char *node, void *key);
extern void func_02050c90(char *node);
void func_02051164(char *object, void *key)
{
    char *node;
    int found;
    unsigned int buffer[4];
    node = *(char **)(object + 0x3c);
    func_0200d824(buffer, key);
    found = func_0200d84c(buffer, &data_020abc94);
    func_0200db44(buffer);
    if (found != 0) {
        for (; node != 0; node = *(char **)(node + 0x54)) {
            if (node != *(char **)(object + 0x40)) {
                func_02050c90(node);
            }
        }
    } else {
        for (; node != 0; node = *(char **)(node + 0x54)) {
            if (func_020509b8(node, key) != 0) {
                func_02050c90(node);
            }
        }
    }
}
