extern "C" {
class Obj {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5(unsigned char v);
};

struct Game {
    char pad0[0xe4c];
    Obj *arr[3];
};

void func_020125a8(struct Game *game, int value)
{
    unsigned int i;

    for (i = 0; i < 3; i++) {
        Obj *o = game->arr[i];
        if (o != 0) {
            o->s5(value);
        }
    }
}
}
