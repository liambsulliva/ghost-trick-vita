int func_02015154(unsigned char *object)
{
    if (object[3] == 0) {
        if ((object[0] & 1) != 0) {
            return 1;
        }
    }
    return 0;
}
