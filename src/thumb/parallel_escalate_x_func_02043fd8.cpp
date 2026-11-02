class Scene02043fd8 {
public:
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
    virtual void v08c(int value);
    virtual void v090();
    virtual void v094(int value);
    virtual void v098();
    virtual void v09c();
    virtual void v0a0();
    virtual void v0a4();
    char pad04[0x28];
    int f2c;
    char pad30[0x12];
    unsigned short f42;
    signed char f44;
    signed char f45;
    char pad46[2];
    void *item;
    char pad4c[4];
    int f50;
};

extern "C" {
extern int func_0202c358(Scene02043fd8 *self);
extern void *func_0202970c(void);
extern int func_02029708(void *object);
extern void func_020440e4(void);
extern void func_020440f0(int first, int second, int third);
extern void func_02044100(int first, int second, int third);
extern void func_02044110(int value);
extern void func_0204411c(int value);
extern void func_02044128(int s, int t);
extern void func_02044148(short x, short y, short z);
extern void func_02044164(void);

void func_02043fd8(Scene02043fd8 *self)
{
    int z;
    int x;
    unsigned int i;

    if (func_0202c358(self) == 0) {
        return;
    }
    if (self->item != 0 && self->f50 == -1) {
        return;
    }
    z = -(func_02029708(func_0202970c()) - self->f42);
    for (i = 0; i < 5; i++) {
        x = self->f2c + (0x80 - i * 0x40) * 0x1000;
        func_020440e4();
        func_020440f0(0, x / 3 >> 5, z);
        func_02044100(0x1000, 0x1555, 0x1000);
        self->v08c(self->f44);
        self->v094(self->f45);
        if (self->f44 > 0) {
            self->v0a4();
            func_02044110(1);
            func_0204411c(0x7fff);
            func_02044128(0, 0);
            func_02044148(-0x1000, 0, 0);
            func_02044128(0, 0x1000);
            func_02044148(-0x1000, -0x2a, 0);
            func_02044128(0x100000, 0x1000);
            func_02044148(0x1000, -0x2a, 0);
            func_02044128(0x100000, 0);
            func_02044148(0x1000, 0, 0);
            func_02044164();
        }
    }
}
}
