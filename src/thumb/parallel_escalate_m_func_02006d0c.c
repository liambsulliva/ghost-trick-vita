extern unsigned long long *func_02006c90(int id);
int func_02006d0c(int id)
{
    unsigned long long *entry;
    entry = func_02006c90(id);
    if (entry != 0 && *entry != 0) {
        return *((unsigned char *)entry + 5);
    }
    return 0xffff;
}
