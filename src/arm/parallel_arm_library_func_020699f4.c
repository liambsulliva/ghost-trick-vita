
extern void func_0207608c();
extern void func_020759c8();

void func_020699f4(unsigned int *obj)
{
    unsigned int arg;
    int (*fn)(unsigned int);

    arg = obj[14];
    fn = (int (*)(unsigned int))obj[13];
    if (fn(arg) != 0) {
        func_0207608c(obj, 1);
        return;
    }
    func_020759c8(obj + 3, 192, 263, func_020699f4, obj);
}
