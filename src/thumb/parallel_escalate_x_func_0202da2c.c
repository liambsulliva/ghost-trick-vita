typedef struct Obj2da2c {
    int f0;
    unsigned char first : 1;
    unsigned char second : 1;
} Obj2da2c;

void func_0202da2c(Obj2da2c *object, int on)
{
    if (on != 0) {
        object->second = 1;
    } else {
        object->second = 0;
    }
}
