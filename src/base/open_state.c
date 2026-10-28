struct State {
    void *methods;
    unsigned short kind;
    unsigned char before_9[3];
    unsigned char flag_9;
    unsigned char before_c[2];
    void *child;
    unsigned char before_18[8];
    unsigned char flag_18;
    unsigned char flag_19;
    unsigned char flag_1a;
};

extern char data_020a2b64;
void func_02009630(struct State *object, int zero, void *part);

struct State *func_02008b88(struct State *object)
{
    object->methods = &data_020a2b64;
    object->kind = 0xfffd;
    object->flag_9 = 0;
    object->child = 0;
    object->flag_18 = 0;
    object->flag_19 = 0;
    object->flag_1a = 0;
    func_02009630(object, 0, (char *)object + 0x14e0);
    return object;
}
