struct Color { unsigned char pad[12]; unsigned short packed; unsigned short pad2; unsigned int red; int green; int blue; };
void func_0200d4d4(struct Color *object, unsigned int value)
{
    unsigned int mask = 0x3e0;
    int green;
    int blue;
    object->red = (value & 0x1f) << 7;
    green = ((int)(value & mask) >> 5) << 7;
    mask <<= 5;
    blue = ((int)(value & mask) >> 10) << 7;
    object->packed = (unsigned short)value;
    object->green = green;
    object->blue = blue;
}
