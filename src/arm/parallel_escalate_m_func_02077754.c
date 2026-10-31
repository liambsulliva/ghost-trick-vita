typedef struct {
    unsigned short x;
    unsigned short y;
    unsigned short touch;
    unsigned short validity;
} TPData;

typedef union {
    struct {
        unsigned int x:12;
        unsigned int y:12;
        unsigned int touch:1;
        unsigned int validity:2;
        unsigned int dummy:5;
    } e;
    unsigned int raw;
    unsigned short halfs[2];
} TPiData;

typedef void (*TPRecvCallback)(int command, int result, unsigned short index);

typedef struct {
    int pad0;
    TPRecvCallback callback;
    TPData buf;
    volatile unsigned short index;
    unsigned short pad12;
    TPData *samplingBufs;
    unsigned short bufSize;
    unsigned short pad1a[14];
    unsigned short state;
    unsigned short err_flg;
    unsigned short command_flg;
} TPStatus;

extern TPStatus data_020d2e84;
extern void func_02075f84(void);

static inline void TPi_CopyTpFromSystemWork(TPData *tpdata)
{
    TPiData tp_data;
    unsigned char *touch_panel = (unsigned char *)0x02ffffaa;
    tp_data.halfs[0] = *(unsigned short *)(&touch_panel[0]);
    tp_data.halfs[1] = *(unsigned short *)(&touch_panel[2]);
    tpdata->x = (unsigned short)tp_data.e.x;
    tpdata->y = (unsigned short)tp_data.e.y;
    tpdata->touch = (unsigned char)tp_data.e.touch;
    tpdata->validity = (unsigned char)tp_data.e.validity;
}

void func_02077754(int tag, unsigned int data, int err)
{
    unsigned short result;
    unsigned short command;

    result = (unsigned short)(data & 0xffff);
    command = (unsigned short)((result & 0x7f00) >> 8);

    if (err) {
        data_020d2e84.err_flg |= (1 << command);
        if (data_020d2e84.callback) {
            (data_020d2e84.callback)(command, 4, 0);
        }
        return;
    }

    if (command == 0x10) {
        data_020d2e84.index++;
        if (data_020d2e84.index >= data_020d2e84.bufSize) {
            data_020d2e84.index = 0;
        }
        TPi_CopyTpFromSystemWork(&data_020d2e84.samplingBufs[data_020d2e84.index]);
        if (data_020d2e84.callback) {
            (data_020d2e84.callback)(command, 0, (unsigned char)data_020d2e84.index);
        }
        return;
    }

    if (!(data & 0x1000000)) {
        return;
    }

    switch ((unsigned char)(result & 0xff)) {
    case 0:
        switch (command) {
        case 0:
            TPi_CopyTpFromSystemWork(&data_020d2e84.buf);
            data_020d2e84.state = 0;
            break;
        case 1:
            data_020d2e84.state = 2;
            break;
        case 2:
            data_020d2e84.state = 0;
            break;
        }
        data_020d2e84.command_flg &= ~(1 << command);
        if (data_020d2e84.callback) {
            (data_020d2e84.callback)(command, 0, 0);
        }
        break;
    case 4:
        result = 3;
        goto common;
    case 2:
        result = 1;
        goto common;
    case 3:
        result = 2;
    common:
        data_020d2e84.err_flg |= (1 << command);
        data_020d2e84.command_flg &= ~(1 << command);
        if (data_020d2e84.callback) {
            (data_020d2e84.callback)(command, (unsigned char)(result & 0xff), 0);
        }
        break;
    case 1:
    default:
        func_02075f84();
    }
}
