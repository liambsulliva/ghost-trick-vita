typedef int code();

extern int func_02011f30(int a);
extern int func_02008828(int a, char *b);
extern int func_02031254(int size);
extern int func_02011f98(int a);
extern void func_02011f90(int a, int b, int c, int d);
extern void func_02011e00(int a, int b);
extern void func_02011e04(int a, int b);
extern void func_02011f34(unsigned char *obj, int a, int b, int c, int d, int e);
extern char data_020ca16c[];

void func_02011fac(unsigned char *obj, int a, int b, int c, int d, int e, unsigned char idx)
{
    int *slots;
    int *items;
    int cur;
    int old;
    int made;
    int fresh;

    if (idx < obj[0xf21]) {
        slots = (int *)(obj + 0xfa4);
        if (slots[idx] != 0) {
            if (func_02011f30(slots[idx]) == 0) {
                items = (int *)(obj + 0xf24);
                if (items[idx] != 0) {
                    if (func_02008828(items[idx], data_020ca16c) != 0) {
                        cur = items[idx];
                        func_02011f90(cur, c, d, e);
                        func_02011e00(cur, a);
                        func_02011e04(cur, b);
                    } else {
                        old = items[idx];
                        if (old != 0) {
                            if (old != 0) {
                                (*(code **)(*(int *)old + 4))(old);
                            }
                            items[idx] = 0;
                        }
                        made = func_02031254(0x1c);
                        if (made != 0) {
                            made = func_02011f98(made);
                        }
                        if (made != 0) {
                            func_02011f90(made, c, d, e);
                            func_02011e00(made, a);
                            func_02011e04(made, b);
                            items[idx] = made;
                        }
                    }
                } else {
                    func_02011f34(obj, a, b, c, d, e);
                }
            }
        } else {
            fresh = func_02031254(0x1c);
            if (fresh != 0) {
                fresh = func_02011f98(fresh);
            }
            if (fresh != 0) {
                func_02011f90(fresh, c, d, e);
                func_02011e00(fresh, a);
                func_02011e04(fresh, b);
                slots[idx] = fresh;
            }
        }
    }
}
