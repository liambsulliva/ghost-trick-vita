typedef struct Track20d7544 {
    char pad[0x32];
    signed char cur[2];
    signed char goal[2];
    signed char limit[2];
} Track20d7544;

int func_ov006_020d7544(Track20d7544 *track, int index, int x)
{
    signed char cur;
    signed char goal;
    int lo = 0;
    int hi = 0;
    goal = track->goal[index];
    cur = track->cur[index];
    if (cur != goal) {
        signed char diff = cur - goal;
        if (diff < 0) {
            lo = diff;
        }
        if (diff > 0) {
            hi = diff;
        }
    }
    if (x < lo + (cur - 1)) {
        return 0;
    }
    if (x > hi + (cur + 1)) {
        return 0;
    }
    if (x < track->limit[index]) {
        return 1;
    }
    return 0;
}
