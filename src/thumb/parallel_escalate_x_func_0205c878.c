typedef struct Obj5c878 {
    char pad[0x4a];
    short y;
} Obj5c878;

typedef struct Info5c878 {
    unsigned char a;
    unsigned char speed;
} Info5c878;

extern Info5c878 data_020cff60;

void func_0205c878(Obj5c878 *object)
{
    object->y = object->y - (short)((data_020cff60.speed + 1) * 2);
    if (object->y < -159) {
        object->y = 159;
    }
}
