union Scales { unsigned short v[12]; };
union Offsets { short v[18]; };
union Frames { unsigned char v[6]; };

class Sprite {
public:
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3();
    virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27();
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void s32();
    virtual void setTimer(int time);
    virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37();
    virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41();
    virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45();
    virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49();
    virtual void s50(); virtual void s51(); virtual void s52();
    virtual void setTransform(int x, int y, int z, int sx, int sy, int sx2, int sy2);
};

struct Actor {
    char pad0[0x88];
    unsigned short level;
    char pad8a[0x2e];
    Sprite *sprite;
    char padbc[0xc];
    unsigned char rank;
};

extern "C" {
extern const Scales data_0209b760;
extern const Offsets data_0209b790;
extern const Frames data_0209b6f8;
extern void func_0204f290(Sprite *sprite, int a, int b);
extern void func_0204b120(Sprite *sprite, int frame);
extern void *func_02009900(void);
extern void func_0202f7e8(void *manager, Sprite *sprite, int a, int b);
}

extern "C" void func_0204b024(Actor *actor)
{
    Scales scales = data_0209b760;
    Offsets offsets = data_0209b790;
    Frames frames = data_0209b6f8;
    unsigned int i;

    actor->rank = actor->level - 0x8015;
    if (actor->rank > 5) {
        actor->rank = 5;
    }
    i = actor->rank;
    actor->sprite->setTransform(offsets.v[i * 3], offsets.v[i * 3 + 1], offsets.v[i * 3 + 2],
                                scales.v[i * 2], scales.v[i * 2 + 1], scales.v[i * 2], scales.v[i * 2 + 1]);
    func_0204f290(actor->sprite, 2, -1);
    func_0204b120(actor->sprite, frames.v[actor->rank]);
    actor->sprite->setTimer(0x150);
    func_0202f7e8(func_02009900(), actor->sprite, 7, 0);
}
