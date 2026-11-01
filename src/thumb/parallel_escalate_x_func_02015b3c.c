typedef struct Node15b3c {
    char pad[0x10];
    unsigned short value;
} Node15b3c;

typedef Node15b3c *(*Method15b3c)(void *self, int arg);

int func_02015b3c(void ****holder, int arg)
{
    void ***object = *holder;
    Node15b3c *node = ((Method15b3c)(*object)[0x58 / 4])(object, arg);
    if (node != 0) {
        return node->value;
    }
    return 0;
}
