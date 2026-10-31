extern unsigned int data_020d1284;
extern unsigned short data_020d1288[9];
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);
extern int func_02075ec4(unsigned int bits);

void func_02075f00(unsigned short bank, unsigned short lockId)
{
    unsigned int workMap;
    int zeroBit;
    unsigned int enabled;

    enabled = func_02075c98();
    workMap = (unsigned int)(bank & data_020d1284 & 0x1ff);
    while (1) {
        zeroBit = 31 - func_02075ec4(workMap);
        if (zeroBit < 0) {
            break;
        }
        workMap &= ~(1 << zeroBit);
        if (data_020d1288[zeroBit] == lockId) {
            data_020d1288[zeroBit] = 0;
            data_020d1284 &= ~(1 << zeroBit);
        }
    }
    func_02075cac(enabled);
}
