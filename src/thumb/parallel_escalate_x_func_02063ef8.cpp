struct Obj63ef8 {
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00c();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01c();
    virtual void v020(int arg);
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
    virtual void v08c(signed char level);
    char pad[0xc6];
    short total;
    short left;
};

extern "C" void func_02063cac(Obj63ef8 *object);

extern "C" void func_02063ef8(Obj63ef8 *object)
{
    func_02063cac(object);
    if (object->total >= 0) {
        object->left = object->left - 1;
        if (object->left <= 0) {
            object->v020(0);
        } else {
            object->v08c((signed char)(object->left * 31 / object->total));
        }
    }
}
