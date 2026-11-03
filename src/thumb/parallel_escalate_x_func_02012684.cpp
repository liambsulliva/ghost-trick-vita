class Mode {
public:
    virtual void s0();
    virtual void s1();
};

struct SceneState {
    unsigned int flags0_low : 6;
    unsigned int ready : 1;
    unsigned int flags0_high : 25;
    unsigned int flags4_low : 16;
    unsigned int layer : 3;
    unsigned int flags4_high : 13;
    char rest[0xc];
};

struct Scene {
    int pad0;
    struct SceneState state;
    char buffer_18[0x40];
    unsigned char cursor;
    unsigned char page;
    char pad5a[2];
    int count;
    char area_60[0x80];
    char area_e0[0x80];
    char area_160[0x980];
    char area_ae0[0x80];
    char area_b60[0x200];
    char pad_d60[0x20];
    char name[8];
    int delay;
    char pad_d8c[0x24];
    int speed;
    int offset;
    char pad_db8[0x18];
    int scale;
    char pad_dd4[0x24];
    char area_df8[0x20];
    char area_e18[0x20];
    int timer;
    char area_e3c[0xc];
    Mode *modes[3];
};

extern "C" {
extern char data_02083ef8[];
extern void func_02011578(void *area, int size);
extern char *func_02078e84(char *dst, const char *src);
extern void *func_02031254(int size);
extern Mode *func_0201fe1c(void *object);
extern Mode *func_0201ffa0(void *object);
extern Mode *func_02020008(void *object);
void func_02012684(Scene *scene);
}

void func_02012684(Scene *scene)
{
    unsigned int i;
    Mode *mode;

    scene->state.ready = 0;
    scene->state.layer = 7;
    scene->page = 0;
    scene->cursor = 0;
    scene->count = 1;
    scene->speed = 0x5000;
    scene->offset = 0;
    scene->scale = 0x1000;
    scene->timer = 0;
    scene->delay = 0x12c;
    func_02011578(&scene->state, sizeof(scene->state));
    func_02011578(scene->buffer_18, sizeof(scene->buffer_18));
    func_02011578(scene->area_df8, sizeof(scene->area_df8));
    func_02011578(scene->area_e18, sizeof(scene->area_e18));
    func_02011578(scene->area_e3c, sizeof(scene->area_e3c));
    func_02078e84(scene->name, data_02083ef8);
    func_02011578(scene->area_60, sizeof(scene->area_60));
    func_02011578(scene->area_e0, sizeof(scene->area_e0));
    func_02011578(scene->area_160, sizeof(scene->area_160));
    func_02011578(scene->area_ae0, sizeof(scene->area_ae0));
    func_02011578(scene->area_b60, sizeof(scene->area_b60));
    for (i = 0; i < 3; i++) {
        if (scene->modes[i] != 0) {
            if (scene->modes[i] != 0) {
                scene->modes[i]->s1();
            }
            scene->modes[i] = 0;
        }
    }
    mode = (Mode *)func_02031254(0x70);
    if (mode != 0) {
        mode = func_0201fe1c(mode);
    }
    scene->modes[0] = mode;
    mode = (Mode *)func_02031254(0xcc);
    if (mode != 0) {
        mode = func_0201ffa0(mode);
    }
    scene->modes[1] = mode;
    mode = (Mode *)func_02031254(0x70);
    if (mode != 0) {
        mode = func_02020008(mode);
    }
    scene->modes[2] = mode;
}
