struct Thing;

struct ThingMethods {
    void *slots[2];
    void (*call)(struct Thing *thing, int zero, void *data);
};

struct Thing {
    struct ThingMethods *methods;
};

struct Holder {
    struct Thing *thing;
};

extern char data_02083a30;

void func_020086ac(struct Holder *holder)
{
    struct Thing *thing = holder->thing;
    thing->methods->call(thing, 0, &data_02083a30);
}
