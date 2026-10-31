struct Slot {
    int value;
    unsigned short half;
    unsigned char a;
    unsigned char b;
};
struct Scene {
    void *vtable;
    unsigned char pad[0x68 - 4];
    int main;
    int listA[2];
    int listB[2];
    int listC[2];
    int listD[2];
    int field8c;
    int field90;
    unsigned short field94;
    unsigned char field96;
    unsigned char pad97[0xe0 - 0x97];
    struct Slot slots[3];
};
extern char data_020a3c3c[];
extern void func_02016c4c(struct Scene *scene);
extern void func_0201a200(struct Scene *scene);

struct Scene *func_02019d4c(struct Scene *scene)
{
    unsigned char i;

    func_02016c4c(scene);
    scene->vtable = data_020a3c3c;
    scene->field8c = 0;
    scene->field90 = 2;
    scene->field94 = 0;
    scene->field96 = 0;
    scene->main = 0;
    for (i = 0; i < 2; i++) {
        scene->listA[i] = 0;
        scene->listB[i] = 0;
        scene->listC[i] = 0;
        scene->listD[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        scene->slots[i].value = 0;
        scene->slots[i].half = 0;
        scene->slots[i].a = 0;
        scene->slots[i].b = 0;
    }
    func_0201a200(scene);
    return scene;
}
