extern unsigned short data_020d0e84;
void func_02071188(unsigned int param_1);

int func_02071b6c(unsigned short *pVRam)
{
    int old;

    old = *pVRam;
    *pVRam = 0;
    data_020d0e84 |= (unsigned short)old;
    func_02071188(old);
    return old;
}
