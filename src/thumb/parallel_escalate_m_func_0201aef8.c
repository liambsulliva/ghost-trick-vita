extern unsigned int data_020b9d0c;
extern unsigned int data_020b9d08;
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *object, unsigned int a, unsigned int b, unsigned int c, int d);

void func_0201aef8(char *object)
{
    if (*(unsigned char *)(object + 0x23) == 1) {
        if (*(unsigned int *)(object + 0x74) > 0x1800) {
            *(unsigned int *)(object + 0x74) = 0x1800;
        }
        if (*(unsigned char *)(object + 0x22) == 1) {
            func_0202f1bc(func_02009c1c(), *(unsigned int *)(object + 0x6c), data_020b9d0c, *(unsigned int *)(object + 0x74), 0x13);
        } else {
        func_0202f1bc(func_02009c1c(), *(unsigned int *)(object + 0x6c), data_020b9d08, *(unsigned int *)(object + 0x74), 5);
        }
    }
}
