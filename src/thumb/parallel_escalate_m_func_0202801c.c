typedef struct {
    unsigned char low : 4;
    unsigned char flag : 1;
} Bits0202801c;

typedef struct {
    char pad[0x140];
    Bits0202801c bits;
    char pad2[0xb];
    unsigned short first;
    unsigned short second;
} Obj0202801c;

void func_0202801c(Obj0202801c *object, unsigned short first, unsigned short second, int low)
{
    object->bits.flag = 1;
    object->bits.low = low & 0xf;
    object->first = first;
    object->second = second;
}
