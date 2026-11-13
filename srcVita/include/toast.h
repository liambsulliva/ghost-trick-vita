#ifndef TOAST_H
#define TOAST_H

void toast_show(const char *key);
void toast_update(unsigned long long elapsed_us);
void toast_draw(void);
int toast_is_showing(void);
float toast_top(void);

#endif
