extern unsigned int func_0202f64c(int *slot);

void func_02010858(int object)
{
    int *slots;
    unsigned int index;

    slots = (int *)(object + 0xdfc);
    index = 2;
    do {
        func_0202f64c(slots + index);
        index = index + 1;
    } while (index < 8);
}
