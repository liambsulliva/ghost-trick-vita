class Child {
public:
    virtual ~Child();
};
extern "C" {
extern void func_0200e3c4(void *object);
extern void func_02031270(void *object);
}
class Part {
public:
    ~Part() { func_0200e3c4(this); }
    static void operator delete(void *object) { func_02031270(object); }
};
extern "C" {
struct Owner {
    Child *children[3];
    Part *parts[3];
};
struct Owner *func_0200db8c(struct Owner *owner)
{
    unsigned int i;
    for (i = 0; i < 3; i++) {
        if (owner->children[i] != 0) {
            delete owner->children[i];
            owner->children[i] = 0;
        }
        if (owner->parts[i] != 0) {
            delete owner->parts[i];
            owner->parts[i] = 0;
        }
    }
    return owner;
}
}
