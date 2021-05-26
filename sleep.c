#include "sleep.h"
#include <sys/time.h>

int msleep(long tms)
{
    struct timeval tv;
    tv.tv_sec  = tms / 1000;
    tv.tv_usec = (tms % 1000) * 1000;
    return select (0, NULL, NULL, NULL, &tv);
}
