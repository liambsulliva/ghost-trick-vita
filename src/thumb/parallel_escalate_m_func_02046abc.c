struct Kid;
struct KidMethods {
    char pad0[0x3c];
    void (*set_position)(struct Kid *kid, int value);
    char pad1[0x4c];
    void (*set_char)(struct Kid *kid, int value);
};
struct Kid {
    struct KidMethods *methods;
};
struct Owner {
    char pad0[0x28];
    void *source;
    struct Kid *kids[4];
    int offset;
};
extern int func_02046b08(void *object);
extern int func_02046b0c(void *object);
void func_02046abc(struct Owner *owner)
{
    int first;
    int second;
    unsigned int i;
    int value;
    first = func_02046b08(owner->source);
    second = func_02046b0c(owner->source);
    for (i = 0; i < 4; i++) {
        owner->kids[i]->methods->set_char(owner->kids[i], (signed char)first);
        value = owner->offset + second;
        if ((i & 2) != 0) {
            value = -value;
        }
        owner->kids[i]->methods->set_position(owner->kids[i], value << 12);
    }
}
