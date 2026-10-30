extern int func_0206fbbc(int *level);
extern int func_0206fbe8(int *level);

void func_0206f9fc(void)
{
    int pvLevel;
    int pjLevel;

    *(volatile unsigned int *)0x04000600 |= 0x8000;
    while (func_0206fbbc(&pvLevel)) {
    }
    while (func_0206fbe8(&pjLevel)) {
    }
    *(volatile unsigned int *)0x04000440 = 3;
    *(volatile unsigned int *)0x04000454 = 0;
    *(volatile unsigned int *)0x04000440 = 0;
    if (pjLevel != 0) {
        *(volatile unsigned int *)0x04000448 = (unsigned int)pjLevel;
    }
    *(volatile unsigned int *)0x04000440 = 2;
    *(volatile unsigned int *)0x04000448 = (unsigned int)pvLevel;
    *(volatile unsigned int *)0x04000454 = 0;
}
