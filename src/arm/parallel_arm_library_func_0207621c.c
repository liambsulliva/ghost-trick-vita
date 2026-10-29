extern char data_02fffc00[];

int func_0207621c(unsigned int bit, int index)
{
    if ((*(unsigned int *)(data_02fffc00 + index * 4 + 0x388) & (1u << bit)) != 0) {
        return 1;
    }
    return 0;
}
