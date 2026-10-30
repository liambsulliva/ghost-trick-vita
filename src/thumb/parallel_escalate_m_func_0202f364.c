typedef void (*Handler0202f364)(int a, int b, int c);
extern Handler0202f364 data_020855d4[];

void func_0202f364(int unused,int *args)
{
    unsigned int index;

    index = args[3];
    if (index != 0) {
        data_020855d4[index](args[0],args[1],args[2]);
    }
}
