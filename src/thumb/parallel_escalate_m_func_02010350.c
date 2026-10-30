extern char data_02083e2c[];
extern char data_02083e34[];
extern void func_02010384(char *object, int value);
extern void func_02010298(char *object, void *item);
extern void func_02012400(char *object, char *a, char *b);

void func_02010350(char *object, void *item)
{
    func_02010384(object, 1);
    func_02010298(object, item);
    *(int *)(object + 0xe58) = 0x19;
    func_02012400(object, data_02083e2c, data_02083e34);
}
