struct Watch {
    char pad0[0x48];
    unsigned char *source;
    int pad4c;
    int value50;
    int value54;
    int value58;
    int pad5c;
    unsigned char byte60;
    unsigned char byte61;
    unsigned char byte62;
    char pad63[9];
    int value6c;
    int value70;
    int pad74;
    unsigned int flags;
};
extern int func_02019094();
extern int func_02019098();
extern int func_02029020();
extern int func_02029024();
extern int func_0202c368();
extern int func_0202c36c();
extern int func_0202c370();
extern int func_0202c374();

void func_020575c8(struct Watch *watch, unsigned char *source)
{
    unsigned int *flags;
    unsigned int value;
    watch->source = source;
    if (source != 0) {
        watch->value50 = func_02029020(source);
        watch->value54 = func_02029024(watch->source);
        watch->value58 = func_0202c368(watch->source);
        watch->byte60 = func_0202c36c(watch->source);
        watch->byte61 = func_0202c370(watch->source);
        watch->byte62 = func_0202c374(watch->source);
        watch->value6c = func_02019094(watch->source) << 12;
        watch->value70 = func_02019098(watch->source) << 12;
        flags = &watch->flags;
        value = watch->flags & ~4;
        watch->flags = value;
        if (watch->value58 != -1) {
            *flags = value | 4;
        }
    }
}
