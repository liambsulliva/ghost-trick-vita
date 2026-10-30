extern char *data_020bbd24;

void func_02020d40(char *object)
{
    char *node;
    char *next;

    node = *(char **)(object + 4);
    while (node != 0) {
        next = *(char **)(node + 0x34);
        if (data_020bbd24 != 0) {
            *(char **)(node + 0x34) = data_020bbd24;
            *(int *)(node + 0x30) = 0;
            *(char **)(data_020bbd24 + 0x30) = node;
        } else {
            *(char **)(node + 0x34) = 0;
            *(int *)(node + 0x30) = 0;
        }
        data_020bbd24 = node;
        node = next;
    }
    *(char **)(object + 4) = 0;
}
