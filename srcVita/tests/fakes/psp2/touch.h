#ifndef FAKE_PSP2_TOUCH_H
#define FAKE_PSP2_TOUCH_H

#define SCE_TOUCH_PORT_FRONT 0
#define SCE_TOUCH_SAMPLING_STATE_START 1
#define SCE_TOUCH_MAX_REPORT 8

typedef struct SceTouchReport {
    unsigned char id;
    short x;
    short y;
} SceTouchReport;

typedef struct SceTouchData {
    unsigned long long timeStamp;
    unsigned int status;
    unsigned int reportNum;
    SceTouchReport report[SCE_TOUCH_MAX_REPORT];
} SceTouchData;

int sceTouchSetSamplingState(int port, int state);
int sceTouchPeek(int port, SceTouchData *data, int count);

#endif
