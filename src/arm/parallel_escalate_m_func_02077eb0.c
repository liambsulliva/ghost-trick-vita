typedef struct {
    unsigned short x;
    unsigned short y;
    unsigned short touch;
    unsigned short validity;
} TPData;

typedef struct {
    long x0;
    long xDotSize;
    long xDotSizeInv;
    long y0;
    long yDotSize;
    long yDotSizeInv;
} TPiCalibrateParam;

typedef struct {
    unsigned char pad[0x34];
    unsigned short calibrate_flg;
} TPiState;

extern TPiState data_020d2e84;
extern TPiCalibrateParam data_020d2ea0;

void func_02077eb0(TPData *disp, const TPData *raw)
{
    TPiCalibrateParam *cal;

    if (!data_020d2e84.calibrate_flg) {
        *disp = *raw;
        return;
    }

    cal = &data_020d2ea0;

    disp->touch = raw->touch;
    disp->validity = raw->validity;

    if (raw->touch == 0) {
        disp->x = 0;
        disp->y = 0;
        return;
    }

    disp->x = (unsigned short)((((long long)(raw->x << 2) - cal->x0) * cal->xDotSizeInv) >> 22);

    if ((short)disp->x < 0) {
        disp->x = 0;
    } else if ((short)disp->x > 255) {
        disp->x = 255;
    }

    disp->y = (unsigned short)((((long long)(raw->y << 2) - cal->y0) * cal->yDotSizeInv) >> 22);

    if ((short)disp->y < 0) {
        disp->y = 0;
    } else if ((short)disp->y > 191) {
        disp->y = 191;
    }
}
