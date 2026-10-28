struct ReleaseOps {
    void *slot_0;
    void *slot_4;
    void *slot_8;
    void *slot_c;
    void (*release)(void *self, void *item);
};

struct Released {
    struct ReleaseOps *methods;
};

struct StateChild {
    void *methods;
    unsigned char before_c[8];
    void *child;
};

extern char data_020a2b64;
void *func_02007bd8(void *value);
void *func_02007b70(void *holder);

struct StateChild *func_02008bb8(struct StateChild *object)
{
    void *step;
    struct Released *released;
    object->methods = &data_020a2b64;
    if (object->child != 0) {
        step = func_02007b70(func_02007bd8(object->child));
        if (step != 0) {
            released = func_02007b70(func_02007bd8(step));
            released->methods->release(released, object->child);
            object->child = 0;
        }
    }
    return object;
}
