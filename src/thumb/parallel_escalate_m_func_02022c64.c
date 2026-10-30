typedef struct {
    int link[2];
    int key;
    unsigned int id : 24;
    unsigned int flags : 8;
} Node02022c64;

extern Node02022c64 *func_020227e0(int object,int value);

int func_02022c64(int object,int value)
{
    int minus;
    Node02022c64 *node;

    minus = 0;
    minus = ~minus;
    if (value == minus) {
        return minus;
    }
    node = func_020227e0(object + 0xf70,value);
    if (node != 0) {
        return node->id;
    }
    return 0;
}
