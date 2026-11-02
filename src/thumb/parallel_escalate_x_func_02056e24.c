typedef struct Panel02056e24 {
    char pad00[0x44];
    signed char f44;
    char pad45[0x33];
    unsigned int pad78 : 4;
    unsigned int lookup : 1;
    unsigned int pad78b : 27;
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
    unsigned short f9e;
    unsigned short fa0;
    unsigned short fa2;
    int fa4;
    int fa8;
    int fac;
    int fb0;
    int fb4;
    int fb8;
    int fbc;
    int fc0;
} Panel02056e24;

extern void *func_0200eb78(void);
extern int func_02022ca4(void *owner, int value);
extern void func_02056fac(int value);
extern void func_02056fb8(int value);
extern void func_02056fc4(int s, int t);
extern void func_02056fe4(int x, int y, int z);
extern void func_02057000(void);

void func_02056e24(Panel02056e24 *self)
{
    if (self->f44 < 1) {
        return;
    }
    func_02056fac(1);
    if (self->lookup) {
        func_02056fb8(func_02022ca4(func_0200eb78(), self->f9c));
        func_02056fc4(self->fa4, self->fa8);
        func_02056fe4(self->f7c, self->f7e, 0);
        func_02056fb8(func_02022ca4(func_0200eb78(), self->f9e));
        func_02056fc4(self->fac, self->fb0);
        func_02056fe4(self->f84, self->f86, 0);
        func_02056fb8(func_02022ca4(func_0200eb78(), self->fa0));
        func_02056fc4(self->fb4, self->fb8);
        func_02056fe4(self->f8c, self->f8e, 0);
        func_02056fb8(func_02022ca4(func_0200eb78(), self->fa2));
        func_02056fc4(self->fbc, self->fc0);
        func_02056fe4(self->f94, self->f96, 0);
        func_02057000();
        return;
    }
    func_02056fb8(self->f9c);
    func_02056fc4(self->fa4, self->fa8);
    func_02056fe4(self->f7c, self->f7e, 0);
    func_02056fb8(self->f9e);
    func_02056fc4(self->fac, self->fb0);
    func_02056fe4(self->f84, self->f86, 0);
    func_02056fb8(self->fa0);
    func_02056fc4(self->fb4, self->fb8);
    func_02056fe4(self->f8c, self->f8e, 0);
    func_02056fb8(self->fa2);
    func_02056fc4(self->fbc, self->fc0);
    func_02056fe4(self->f94, self->f96, 0);
    func_02057000();
}
