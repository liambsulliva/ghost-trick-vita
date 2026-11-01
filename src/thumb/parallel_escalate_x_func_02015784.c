extern void func_020152f4(unsigned char *object);

void func_02015784(unsigned char *object)
{
    if (object[0x10] == 0) {
        object[0x62] = 1;
        object[0x10] = 1;
    }
    func_020152f4(object);
}
