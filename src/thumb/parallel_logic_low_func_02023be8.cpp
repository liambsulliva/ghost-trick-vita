class Inner02023be8 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
};

class Object02023be8 {
public:
    virtual Inner02023be8 *slot0();
};

extern "C" {
int func_02023be8(Object02023be8 *object)
{
    Inner02023be8 *inner;

    if (object != 0) {
        inner = object->slot0();
        if (inner != 0) {
            inner->slot5();
            if (inner != 0) {
                inner->slot1();
            }
            return 1;
        }
    }
    return 0;
}
}
