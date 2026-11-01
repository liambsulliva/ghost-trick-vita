struct Obj4437c {
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00c();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01c();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02c();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03c();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04c();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05c();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06c();
    virtual void v070();
    virtual void v074();
    virtual void v078();
    virtual void v07c();
    virtual void v080();
    virtual void v084();
    virtual void v088();
    virtual void v08c();
    virtual void v090();
    virtual void v094();
    virtual void v098();
    virtual void v09c();
    virtual void v0a0();
    virtual void v0a4();
    virtual void va8(int id, int a, int b, int c, int d, int e, int f);
    char pad[0xc];
    unsigned char state;
    char pad2[2];
    unsigned char delay;
    char pad3[0xb4];
    int mode;
};

extern "C" void func_0204437c(Obj4437c *obj)
{
    switch (obj->state) {
    case 4:
    case 5:
        if (obj->mode == 1) {
            obj->va8(0x273, 0, 1, 0, 0, -1, 0);
        } else if (obj->mode == 2) {
            obj->va8(0x27d, 0, 1, 0, 0, -1, 0);
        }
        obj->state = 5;
        obj->delay = 10;
        break;
    }
}
