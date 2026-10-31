struct Object {
    void *methods;
    unsigned char byte4;
    unsigned char byte5;
    unsigned char pad6[0x80];
    unsigned char flag0 : 1;
    unsigned char flag1 : 1;
    unsigned char flag2 : 1;
    unsigned char flag3 : 1;
    unsigned char byte87;
    int word88;
    int word8c;
    unsigned char bytes90[4];
    int words94[4];
    int wordsa4[4];
    int wordb4;
    unsigned char padb8[0x24];
    int worddc;
    int worde0;
};

extern char data_020a5198;
extern struct Object *func_0202b2e0(struct Object *object);

struct Object *func_0202cc84(struct Object *object)
{
    unsigned int i;

    func_0202b2e0(object);
    object->methods = &data_020a5198;
    object->flag0 = 0;
    object->flag1 = 0;
    object->flag2 = 0;
    object->flag3 = 0;
    object->byte87 = 0;
    object->word88 = 0;
    object->word8c = -1;
    object->wordb4 = 0;
    object->worddc = 0;
    object->worde0 = 0;
    object->byte5 = 0x27;
    for (i = 0; i < 4; i++) {
        object->bytes90[i] = 0;
        object->words94[i] = 0;
        object->wordsa4[i] = 0;
    }
    return object;
}
