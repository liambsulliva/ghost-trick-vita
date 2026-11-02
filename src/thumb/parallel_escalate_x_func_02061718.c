typedef struct Obj61718 {
    char pad0[0x7c];
    unsigned char low7c : 6;
    unsigned char busy : 1;
    unsigned char high7c : 1;
    unsigned char held : 1;
    unsigned char mid7d : 2;
    unsigned char linked : 1;
    unsigned char high7d : 4;
    char pad7e;
    unsigned char count;
    char pad80[0x54];
    void *buffer;
    char padd8[4];
    void *panel;
    char pade0[0x14];
    void *first;
    void *second;
    char padfc[8];
    unsigned short timer;
} Obj61718;

extern char data_0209dc70;
extern char data_0209dc84;
extern char data_0209dc34;
extern int func_020616dc(Obj61718 *obj, void *thing);
extern int func_020616f4(Obj61718 *obj, void *thing);
extern void *func_02008444(void);
extern int func_0201132c(void *game);
extern int func_02058ba4(void *object);
extern void *func_02027684(void *game);
extern void *func_0202768c(void *game);
extern void func_020266e8(void *game, void *thing);
extern void func_02011598(void *game);
extern void func_02072fa8(void *dst, int value, int size);
extern void func_0205f074(void *panel, int value);
extern void *func_02014220(void *object);
extern void func_0200da04(void *object, void *a, void *b, void *c);
extern void func_02060d54(Obj61718 *obj, int state);

void func_02061718(Obj61718 *obj, void *thing)
{
    int found;
    int separate;
    int extra;
    void *partner;
    void *a;
    void *b;

    if (thing == 0) {
        return;
    }
    found = 0;
    separate = 1;
    extra = 0;
    if (func_020616dc(obj, thing) != 0) {
        found = 1;
    } else if (func_020616f4(obj, thing) != 0) {
        found = 1;
        extra = 1;
    }
    if (func_0201132c(func_02008444()) != 0 && thing != 0 && obj->first != 0) {
        partner = obj->second;
        if (partner != 0 && func_02058ba4(obj->first) == func_02058ba4(partner) && func_02058ba4(obj->first) != 0) {
            found = 1;
            separate = 0;
        }
    }
    if (func_0201132c(func_02008444()) != 0 && thing != 0 && thing == func_02027684(func_02008444())) {
        found = 1;
        separate = 1;
    }
    if (func_0201132c(func_02008444()) == 0 && thing != 0 && thing == func_0202768c(func_02008444())) {
        found = 1;
        separate = 1;
    }
    if (found == 0) {
        return;
    }
    func_020266e8(func_02008444(), thing);
    func_02011598(func_02008444());
    func_02072fa8(obj->buffer, 0, obj->count * 12);
    func_0205f074(obj->panel, 0x3f0);
    obj->timer = 0x3ff;
    obj->busy = 0;
    if (func_0201132c(func_02008444()) != 0 && separate == 0) {
        a = func_02014220(obj->first);
        b = func_02014220(obj->second);
        func_0200da04(a, &data_0209dc70, &data_0209dc84, &data_0209dc34);
        func_0200da04(b, &data_0209dc70, &data_0209dc84, &data_0209dc34);
        obj->linked = 0;
        func_02060d54(obj, 5);
        return;
    }
    obj->linked = 0;
    obj->held = 0;
    if (extra) {
        obj->held = 1;
    }
    func_02060d54(obj, 4);
}
