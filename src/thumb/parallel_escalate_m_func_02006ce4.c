extern unsigned long long *func_02006c90(int id);
int func_02006ce4(int id)
{
    unsigned long long *entry;
    entry = func_02006c90(id);
    if (entry != 0 && *entry != 0) {
        return *((unsigned char *)entry + 2);
    }
    return 0xffff;
}
