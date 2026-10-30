struct Flags {
    unsigned int low : 8;
    unsigned int on : 1;
    unsigned int high : 23;
};
struct Object {
    int pad[2];
    struct Flags flags;
};
extern void func_02010f0c(struct Object *object, int index, unsigned char mode);

void func_02011cdc(struct Object *object, int on)
{
    int mode;
    object->flags.on = on;
    mode = 5;
    if (on == 0) {
        mode = 0;
    }
    func_02010f0c(object, 1, mode);
}
