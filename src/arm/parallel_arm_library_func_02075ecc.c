extern unsigned int data_020d1284;
extern unsigned short data_020d1288[9];

void func_02075ecc(void)
{
    int index;

    index = 0;
    data_020d1284 = 0;
    do {
        data_020d1288[index] = 0;
        index = index + 1;
    } while (index < 9);
}
