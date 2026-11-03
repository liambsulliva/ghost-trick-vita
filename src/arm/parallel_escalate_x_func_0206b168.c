extern int func_02075c98(void);
extern void func_02075cac(int enabled);
extern void func_0206b090(int tag, unsigned int data, int err);

void func_0206b168(unsigned int id)
{
    volatile unsigned int ipl_id;
    int enabled;

    ipl_id = *(unsigned int *)0x02fffc00;
    if (id == ipl_id) {
        return;
    }
    enabled = func_02075c98();
    func_0206b090(14, 17, 0);
    func_02075cac(enabled);
}
