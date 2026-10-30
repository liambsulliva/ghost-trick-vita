struct Obj {
    unsigned int f0;
    unsigned int f4;
    unsigned int f8;
    unsigned int fc;
};

int func_0206d714(struct Obj *obj, unsigned int command, int block);

int func_0206db58(int unused0, struct Obj *obj)
{
    int ret;
    unsigned int flags;

    ret = func_0206d714(obj, 8, 1);
    flags = obj->fc;
    obj->f4 = 0;
    obj->fc = flags & 0xffffffcf;
    return ret;
}
