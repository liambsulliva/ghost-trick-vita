extern int func_02052618();
extern char data_020a91e0;

char *func_0204714c(char *object)
{
    int value;

    func_02052618(object);
    *(int *)object = (int)&data_020a91e0;
    *(unsigned char *)(object + 0xb4) = 0;
    value = 1;
    *(unsigned char *)(object + 0xb5) = value;
    value -= 2;
    *(short *)(object + 0xb6) = value;
    return object;
}
