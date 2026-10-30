extern void *func_020681b0(void *startAddress, void *endAddress, unsigned short optFlag);

void *func_020685e0(void *startAddress, unsigned int size, unsigned short optFlag)
{
    void *endAddress;

    endAddress = (void *)((size + (unsigned int)startAddress) & ~3);
    startAddress = (void *)(((unsigned int)startAddress + 3) & ~3);
    if ((unsigned int)startAddress > (unsigned int)endAddress
        || (unsigned int)endAddress - (unsigned int)startAddress < 76) {
        return 0;
    }
    return func_020681b0(startAddress, endAddress, optFlag);
}
