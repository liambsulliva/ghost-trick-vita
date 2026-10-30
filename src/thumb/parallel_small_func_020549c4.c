extern char *func_02008444(void);
extern int func_020151b0(char *object);
extern int func_02054aec(char *object);
extern int func_02054a6c(char *object);

void func_020549c4(char *object)
{
    if (func_020151b0(func_02008444()) == 0) {
        if (*(int *)(object + 0x38) < 0) {
            *(int *)(object + 0x38) = ~0x37;
            *(unsigned char *)(object + 0x10) = 1;
            func_02054aec(object);
            func_02054a6c(object);
        }
    }
}
