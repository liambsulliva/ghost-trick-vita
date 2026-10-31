class Unit {
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
    virtual void v08c();
    virtual void v090();
    virtual void v094();
    virtual void v098();
    virtual void v09c();
    virtual void v0a0();
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
    virtual void v170();
    virtual void v174();
    virtual int v178(unsigned int a, int b);
};

extern "C" {
extern void func_0201778c(Unit *object);
extern void func_02017714(Unit *object, int a);
extern void func_0201c6c4(void *a, unsigned int b);
extern void func_0201cf2c(void *a, unsigned int b);
extern void func_02016fbc(void *a, int b);
extern void func_0201aa14(Unit *object);
extern void func_0201c634(void *a, unsigned int b, int c, int d, int e);
extern void func_0201a108(void *a);
extern void func_0201aa4c(Unit *object);
extern void func_0201aa78(Unit *object);
extern void func_02016e28(Unit *object);
extern void func_0201a254(Unit *object);
extern void func_0201a3ac(Unit *object);
extern char data_020b9df4;

int func_0201a04c(Unit *object, unsigned char value, unsigned int seed)
{
    char *p;

    p = (char *)object;
    *(unsigned short *)(p + 0x20) = 1;
    func_0201778c(object);
    func_02017714(object, 1);
    *(unsigned short *)(p + 0x2a) = 0xf;
    *(unsigned char *)(p + 0x96) = value;
    *(unsigned short *)(p + 0x94) = 0;
    *(unsigned int *)(p + 0x18) = seed;
    *(unsigned int *)(p + 0x14) = seed;
    func_0201c6c4(&data_020b9df4, *(unsigned int *)(p + 0x10));
    func_0201cf2c(&data_020b9df4, *(unsigned int *)(p + 0x60));
    func_02016fbc(&data_020b9df4, 3);
    func_0201aa14(object);
    *(unsigned short *)(p + 0x38) = object->v178(*(unsigned int *)(p + 0x14), 1);
    if ((int)*(unsigned short *)(p + 0x38) > 3) {
        *(unsigned short *)(p + 0x38) = 3;
    }
    func_0201c634(&data_020b9df4, *(unsigned int *)(p + 0xe0), 0x100, 0x14, 2);
    func_0201a108(&data_020b9df4);
    func_0201aa4c(object);
    func_0201aa78(object);
    func_02016e28(object);
    if (*(unsigned char *)(p + 0x96) <= 1) {
        func_0201a254(object);
    } else if (*(unsigned char *)(p + 0x96) < 5) {
        func_0201a3ac(object);
    }
    return *(int *)(p + 0x68);
}
}
