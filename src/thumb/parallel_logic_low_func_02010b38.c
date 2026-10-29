struct Flags {
    unsigned int before : 9;
    unsigned int bit9 : 1;
    unsigned int bit10 : 1;
};
unsigned int func_02010b38(int object)
{
    if ((((struct Flags *)(object + 8))->bit10 != 0) ||
          (((struct Flags *)(object + 8))->bit9 != 0) ||
          (*(unsigned char *)(object + 26) != 0)) {
        return 1;
    }
    return 0;
}
