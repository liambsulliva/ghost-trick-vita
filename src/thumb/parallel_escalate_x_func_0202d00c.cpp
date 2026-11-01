struct Entry2d00c {
    int key;
    int f4;
    unsigned char flags;
    char pad[3];
    int body;
};

struct Obj2d00c {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c(int *body, int a, int b, int c, int d);
    char pad4[2];
    unsigned char b6;
    char pad7[0x81];
    int current;
    int previous;
    char pad90[0x40];
    unsigned int count;
    int *offsets;
    char *base;
};

extern "C" void func_0202d00c(Obj2d00c *obj)
{
    unsigned int i;
    obj->previous = obj->current;
    obj->b6 = 0;
    for (i = 0; i < obj->count; i++) {
        Entry2d00c *entry = (Entry2d00c *)(obj->base + obj->offsets[i]);
        int *body = &entry->body;
        if (entry->key == obj->current && (entry->flags & 1)) {
            obj->v4c(body, 0, 0, 0, 0);
        }
    }
}
