extern int func_02075c98();
extern void func_02075cac();
extern int *func_02073b68();
extern void func_02073cc4();

void func_02074120(int *p)
{
    int saved;
    int *n;
    saved = func_02075c98();
    if (p[0] != 0) {
        while (p[0] != 0) {
            n = func_02073b68(p);
            n[25] = 1;
            n[30] = 0;
            n[32] = 0;
            n[31] = 0;
        }
        p[1] = 0;
        p[0] = 0;
        func_02073cc4(0);
    }
    func_02075cac(saved);
}
