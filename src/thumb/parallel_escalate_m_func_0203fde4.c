struct Flags0203fde4 {
    unsigned char low : 5;
    unsigned char high : 3;
};

struct Object0203fde4 {
    unsigned char pad[0x1e];
    struct Flags0203fde4 flags;
};

void func_0203fde4(struct Object0203fde4 *object, unsigned char value)
{
    object->flags.high = value;
}
