struct Filter0202c1a0 {
    virtual void v000();
    virtual void v004();
    virtual int v008(int value, int flag);
};

struct View0202c1a0 {
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
    virtual int v0a0();
    virtual void v0a4();
    virtual void v0a8();
    virtual void v0ac();
    virtual void v0b0();
    virtual void v0b4();
    virtual void v0b8();
    virtual void v0bc();
    virtual void v0c0();
    virtual void v0c4();
    virtual void v0c8();
    virtual void v0cc();
    virtual void v0d0();
    virtual void v0d4();
    virtual void v0d8();
    virtual void v0dc();
    virtual void v0e0();
    virtual void v0e4();
    virtual void v0e8();
    virtual void v0ec();
    virtual void v0f0();
    virtual void v0f4();
    virtual void v0f8();
    virtual void v0fc();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10c();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11c();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12c();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13c();
    virtual void v140();
    virtual void v144();
    virtual void v148();
    virtual void v14c();
    virtual void v150();
    virtual void v154();
    virtual void v158();
    virtual void v15c();
    virtual void v160();
    virtual void v164();
    virtual void v168();
    virtual void v16c();
    virtual int v170();
};

struct Scene0202c1a0 {
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
    char pad04[0x3e];
    unsigned short f42;
    signed char f44;
    char pad45[3];
    void *item;
    char pad4c[4];
    int f50;
    int f54;
    int f58;
    char pad5c[4];
    unsigned char f60;
    unsigned char f61;
    unsigned char f62;
    char pad63[0x19];
    short f7c;
    short f7e;
    char pad80[4];
    short f84;
    short f86;
    char pad88[4];
    short f8c;
    short f8e;
    char pad90[4];
    short f94;
    short f96;
    char pad98[4];
    unsigned short f9c;
    char pad9e[6];
    int fa4;
    int fa8;
    int fac;
    int fb0;
    int fb4;
    int fb8;
    int fbc;
    int fc0;
    char padc4[4];
    void *fc8;
};

typedef struct Clock0202c1a0 {
    char pad[0x2c];
    int f2c;
} Clock0202c1a0;

extern "C" {
extern Clock0202c1a0 data_020a5078;
extern int func_0202c358(Scene0202c1a0 *self);
extern int func_02029020(void *item);
extern int func_02029024(void *item);
extern int func_0202c368(void *item);
extern int func_0202c36c(void *item);
extern int func_0202c370(void *item);
extern int func_0202c374(void *item);
extern void *func_0202970c(void);
extern int func_02029708(void *object);
extern void func_0206f77c(Clock0202c1a0 *clock);
extern void func_0202c378(int value);
extern View0202c1a0 *func_0202c384(void *object);
extern void *func_0200eb78(void);
extern int func_02022d04(void *owner, int a, int b);
extern Filter0202c1a0 *func_0202c38c(View0202c1a0 *view);
extern void *func_02008444(void);
extern int func_0202c394(void *object);
extern int func_02022ca4(void *owner, int value);
extern void func_0202c3a4(int value);
extern void func_0202c3b0(int a, int b);
extern void func_0202c3d0(int a, int b, int c);
extern void func_0202c3ec(void);

void func_0202c1a0(Scene0202c1a0 *self)
{
    View0202c1a0 *view;
    Filter0202c1a0 *filter;
    int value;
    int shown;
    int result;
    int over;
    int offset;

    if (func_0202c358(self) == 0) {
        return;
    }
    if (self->f44 < 1) {
        return;
    }
    if (self->item != 0) {
        self->f50 = func_02029020(self->item);
        self->f54 = func_02029024(self->item);
        self->f58 = func_0202c368(self->item);
        self->f60 = func_0202c36c(self->item);
        self->f61 = func_0202c370(self->item);
        self->f62 = func_0202c374(self->item);
    }
    if (self->item != 0 && self->f50 == -1) {
        return;
    }
    data_020a5078.f2c = -(func_02029708(func_0202970c()) - self->f42);
    func_0206f77c((Clock0202c1a0 *)0x020a5078);
    self->v0a4();
    func_0202c378(1);
    view = func_0202c384(self->fc8);
    value = func_02022d04(func_0200eb78(), self->f9c, view->v0a0());
    shown = view->v170();
    filter = func_0202c38c(view);
    if (shown != 0 && func_0202c394(func_02008444()) != 0 && filter != 0) {
        value = filter->v008(value, 0);
    }
    result = func_02022ca4(func_0200eb78(), value);
    over = 0;
    if (self->f7e < self->f8e) {
        over = 1;
    }
    if (self->f86 > self->f96) {
        over = 1;
    }
    offset = 0;
    if (over) {
        offset = 0x1000;
    }
    func_0202c3a4(result);
    func_0202c3b0(self->fa4, self->fa8);
    func_0202c3d0(self->f7c, self->f7e, 0);
    func_0202c3b0(self->fac, self->fb0 - offset);
    func_0202c3d0(self->f84, self->f86, 0);
    func_0202c3b0(self->fb4, self->fb8 - offset);
    func_0202c3d0(self->f8c, self->f8e, 0);
    func_0202c3b0(self->fbc, self->fc0);
    func_0202c3d0(self->f94, self->f96, 0);
    func_0202c3ec();
}
}
