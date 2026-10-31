extern int func_0206ef28(int param_1, int param_2);

void func_0200d4f8(int *param_1)
{
    int max;
    int min;
    int delta;
    int hue;

    if (param_1[4] <= param_1[5] && param_1[4] <= param_1[6]) {
        min = param_1[4];
        max = (param_1[5] < param_1[6]) ? param_1[6] : param_1[5];
    } else if (param_1[5] <= param_1[6]) {
        min = param_1[5];
        max = (param_1[4] < param_1[6]) ? param_1[6] : param_1[4];
    } else {
        min = param_1[6];
        max = (param_1[4] < param_1[5]) ? param_1[5] : param_1[4];
    }
    param_1[2] = max;
    if (max == 0) {
        param_1[0] = 0;
        param_1[1] = 0;
        return;
    }
    delta = max - min;
    param_1[1] = func_0206ef28(delta, max);
    if (max == param_1[4]) {
        hue = func_0206ef28(param_1[5] - param_1[6], delta);
    } else if (max == param_1[5]) {
        hue = func_0206ef28(param_1[6] - param_1[4], delta) + 0x2000;
    } else {
        hue = func_0206ef28(param_1[4] - param_1[5], delta) + 0x4000;
    }
    hue = hue * 60;
    if (hue < 0) {
        hue = hue + 0x168000;
    }
    param_1[0] = hue;
}
