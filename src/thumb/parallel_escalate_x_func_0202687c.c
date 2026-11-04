struct Obj {
    int pad[3];
    unsigned int lo : 16;
    unsigned int cur : 3;
    unsigned int prev : 3;
    unsigned int hi : 10;
};

void func_0202687c(void *game)
{
    struct Obj *o;
    o = (struct Obj *)game;
    if (o->cur < 7) {
        o->prev = o->cur;
        o->cur = o->cur + 1;
    }
}
