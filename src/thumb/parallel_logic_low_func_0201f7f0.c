extern int func_0201f698(int size);
extern int func_0201f598(int object);

int func_0201f7f0(int owner)
{
    int object;

    object = func_0201f698(0xc);
    if (object != 0) {
        object = func_0201f598(object);
    }
    if (object != 0) {
        *(int *)(object + 8) = owner;
        *(unsigned char *)(object + 7) = *(unsigned char *)(owner + 7);
    }
    return object;
}
