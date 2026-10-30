extern int data_020b9d0c;
extern int data_020b9d08;
extern int *func_02009c1c(void);
extern void func_0202f1bc(int *table, int a, int b, int c, int d);

void func_0201888c(unsigned char *object)
{
    if (object[0x23] == 1) {
        if (*(unsigned int *)(object + 0x74) > 0x1800) {
            *(unsigned int *)(object + 0x74) = 0x1800;
        }
        if (object[0x22] == 1) {
            func_0202f1bc(func_02009c1c(), *(int *)(object + 0x6c), data_020b9d0c, *(int *)(object + 0x74), 0x13);
            return;
        }
        func_0202f1bc(func_02009c1c(), *(int *)(object + 0x6c), data_020b9d08, *(int *)(object + 0x74), 5);
    }
}
